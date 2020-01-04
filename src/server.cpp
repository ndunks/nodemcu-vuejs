#include "header.h"
#include <EEPROM.h>
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <FS.h>

ESP8266WebServer server(80);
WiFiClient client;
char method[8] = {0};
char path[100] = {0};
DNSServer dnsServer;
bool server_islogin()
{
    if (
        (server.hasHeader("authorization") && server.header("authorization").equals(config->password)) ||
        (server.hasArg("auth") && server.arg("auth").equals(config->password)))
    {
        return true;
    }
    Serial.println("Blocked by GUARD");
    server.send(403, "text/plain", "Not authorized");
    return false;
}
void server_handler()
{
    // Captive portal
    server.on("/hotspot-detect.html", []() {
        server.sendHeader("Location", "http://wifi-setup.id/");
        server.send(302);
    });
    server.on("/", []() {
        digitalWrite(ledPin, LOW);
        File f = SPIFFS.open("/index.html.gz", "r");
        server.streamFile(f, "text/html");
        f.close();
        digitalWrite(ledPin, HIGH);
    });

    server.on("/heap", HTTP_GET, []() {
        String body(String(ESP.getFreeHeap()));
        server.send(200, "text/plain", body);
        body.clear();
    });

    server.on("/gpio", HTTP_GET, []() {
        String body(String((uint32_t)(((GPI | GPO) & 0xFFFF) | ((GP16I & 0x01) << 16))));
        body += "|" + String(analogRead(A0));
        server.send(200, "text/plain", body);
        body.clear();
    });

    server.on("/ping", HTTP_GET, []() {
        server.sendHeader("Access-Control-Allow-Origin", "*");
        if (!server_islogin())
            return;
        server.send(200, "text/plain", "OK");
    });

    server.on("/reboot", HTTP_GET, []() {
        if (!server_islogin())
            return;
        system_restart();
    });

    server.on("/set-wifi", HTTP_GET, []() {
        if (!server_islogin())
            return;

        String body;
        int id = server.arg("id").toInt();
        String pass = server.arg("pass");
        wl_status_t state = WiFi.begin(WiFi.SSID(id).c_str(), pass.c_str(), WiFi.channel(id));
        WiFi.setAutoConnect(true);
        server.send(200, "text/plain", String(state));
    });

    server.on("/set-password", HTTP_GET, []() {
        if (!server_islogin())
            return;
        String body;
        if (server.hasArg("password"))
        {
            String password = server.arg("password");
            if (password.length() > 8)
            {
                body = "Password terlalu panjang, maks 8 huruf";
            }
            else
            {
                strncpy(config->password, password.c_str(), password.length());
                if (EEPROM.commit())
                {
                    body = "OK, Berhasil disimpan";
                }
                else
                {
                    body = "Gagal menyimpan";
                }
            }
        }
        else
        {
            body = "Password tidak boleh kosong";
        }
        server.send(200, "text/plain", body);
    });

    server.on("/scan", HTTP_GET, []() {
        if (!server_islogin())
            return;

        String body;
        int found = WiFi.scanNetworks();
        if (found == WIFI_SCAN_FAILED)
        {
            body += "FAILED";
        }
        else
        {
            for (int i = 0; i < found; i++)
            {
                body += String(WiFi.RSSI(i)) + "\t";
                switch (WiFi.encryptionType(i))
                {
                case ENC_TYPE_NONE:
                    body += "none";
                    break;
                case ENC_TYPE_WEP:
                    body += "wep";
                    break;
                case ENC_TYPE_TKIP:
                    body += "tkip";
                    break;
                case ENC_TYPE_CCMP:
                    body += "ccmp";
                    break;
                case ENC_TYPE_AUTO:
                    body += "auto";
                    break;
                };
                body += "\t" + WiFi.SSID(i);
                body += "\n";
            }
        };
        server.send(200, "text/plain", body);
        body.clear();
    });
}
void server_begin()
{
    Serial.println("Server started");
    Serial.print("http://");
    Serial.print(WiFi.localIP());
    Serial.println("/");
    dnsServer.setErrorReplyCode(DNSReplyCode::Refused);
    dnsServer.start(53, "*", local_IP);
    if (!SPIFFS.begin())
    {
        Serial.write("[!] FileSystem ERROR\n");
        ticker.attach_ms(200, blink_error);
        return;
    }
    
    server_handler();
    server.serveStatic("/", SPIFFS, "/", "public, max-age=86400");
    server.begin();
}

void server_loop()
{
    dnsServer.processNextRequest();
    server.handleClient();
}
