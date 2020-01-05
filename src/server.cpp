#include "header.h"

ESP8266WebServer server;
WiFiClient client;
char method[8] = {0};
char path[100] = {0};
DNSServer dnsServer;
Handler handler;

bool server_guard()
{
    if (
        (server.hasHeader("authorization") && server.header("authorization").equals(config->password)) ||
        (server.hasArg("auth") && server.arg("auth").equals(config->password)))
    {
        return true;
    }
    Serial.println("Blocked by GUARD");
    server.send(403, "text/plain", "Unauthorized");
    return false;
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
    server.addHandler(&handler);
    server.serveStatic("/", SPIFFS, "/", "public, max-age=86400");

    server.onNotFound([]() {
        String host = server.hostHeader();
        Serial.printf("Not found: %s %s\n", host.c_str(), server.uri().c_str());
        if (host.length() && (host.equalsIgnoreCase(WiFi.hostname()) ||
                              host.equals(WiFi.localIP().toString()) ||
                              host.equals(WiFi.softAPIP().toString())))
        {
            Serial.println("SPA");
            handle_index(*((String *)NULL), server.method());
        }
        else
        {
            server.sendHeader("Location", "http://" + WiFi.hostname() + "/");
            server.send(302, "text/html", "Redirecting...");
            Serial.println("Handled Captive Portal");
        }
    });
    server.begin();
}

void server_loop()
{
    dnsServer.processNextRequest();
    server.handleClient();
}
