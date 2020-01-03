#include "header.h"
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <FS.h>

ESP8266WebServer server(80);
WiFiClient client;
char method[8] = {0};
char path[100] = {0};
DNSServer dnsServer;
// TODO: use mimeTable
String getContentType(String filename)
{
    if (filename.endsWith(".html"))
        return "text/html";
    else if (filename.endsWith(".css"))
        return "text/css";
    else if (filename.endsWith(".js"))
        return "application/javascript";
    else if (filename.endsWith(".ico"))
        return "image/x-icon";
    else if (filename.endsWith(".gz"))
        return "application/x-gzip";
    return "text/plain";
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
    if (!SPIFFS.exists("/index.html"))
    {
        Serial.write("!! No /index.html\n");
    }
    // Captive portal
    server.on("/hotspot-detect.html", []() {
        server.sendHeader("Location", "http://wifi-setup.id/");
        server.send(302);
    });

    server.onNotFound([]() {
        digitalWrite(ledPin, LOW);
        String host = server.hostHeader();
        String path = server.uri();
        Serial.printf("method %d: %s\n%s\n", server.method(), host.c_str(), path.c_str());
        if (host.indexOf("apple") >= 0 || host.indexOf("google") >= 0 || host.indexOf("android") >= 0 || host.indexOf("gstatic") >= 0)
        {
            server.sendHeader("Location", "http://wifi-setup.id/");
            server.send(302);
        }
        else
        {

            if (path.equals("/"))
            {
                path += "index.html";
            }
            if (SPIFFS.exists(path))
            {

                // Todo: optimize cache control, load file from single bundle or pre GZIP!
                File file = SPIFFS.open(path, "r");
                size_t sent = server.streamFile(file, getContentType(path));
                file.close();
                Serial.printf(" %s [OK] %d\n", path.c_str(), sent);
            }
            else
            {
                server.send(404, "text/plain", "404: Ora Ketemu");
                Serial.printf(" [NOTFOUND]\n");
            }
        }

        digitalWrite(ledPin, HIGH);
    });
    server.begin();
}

void server_loop()
{
    dnsServer.processNextRequest();
    server.handleClient();
}
