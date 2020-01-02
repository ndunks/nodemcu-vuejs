#include "header.h"
#include <DNSServer.h>

WiFiServer server(80);
WiFiClient client;
char method[8] = {0};
char path[100] = {0};
DNSServer dnsServer;

void server_begin()
{
    Serial.println("Server started");
    Serial.print("http://");
    Serial.print(WiFi.localIP());
    Serial.println("/");
    dnsServer.setErrorReplyCode(DNSReplyCode::NoError);
    dnsServer.start(53, "*", local_IP);
    server.begin();
}

void server_loop()
{
    dnsServer.processNextRequest();
    client = server.available();
    if (!client)
    {
        return;
    }
    Serial.print("Client: ");
    Serial.println(client.remoteIP());

    int loopCount = 0;
    while (!client.available())
    {
        Serial.print(".");
        delay(10);
        if (++loopCount > 100)
        {
            Serial.println("Tired waiting :-(");
            return;
        }
    }
    Serial.printf("%d\n", client.available());
    //client.disableKeepAlive();

    client.readBytesUntil(' ', method, sizeof(method) - 1);
    client.readBytesUntil(' ', path, sizeof(path) - 1);
    Serial.printf("%s %s\n", method, path);
    client.flush();
    client.println("HTTP/1.1 200 OK");
    client.println("Content-Type: text/plain");
    client.println("Content-Length: 11");
    client.println("Connection: Close");
    client.println("");
    client.print("HELLO WORLD");
}