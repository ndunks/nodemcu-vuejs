#include "header.h"
bool ApiHandler::canHandle(HTTPMethod method, String path)
{
    if (method == HTTP_OPTIONS)
    {
        return true;
    }
    return path.startsWith(prefix);
}

bool ApiHandler::handle(ESP8266WebServer &server, HTTPMethod method, String path)
{
    Serial.printf("ApiHandler: %s\n", path.c_str());
    server.sendHeader("Access-Control-Allow-Origin", "*");
    if (method == HTTP_OPTIONS)
    {
        server.sendHeader("Access-Control-Allow-Methods", "GET,POST");
        server.sendHeader("Access-Control-Allow-Headers", "*");
        // 60 * 60 * 60 * 30 = 6480000
        server.sendHeader("Access-Control-Max-Age", "6480000");
        server.send(204, "text/plain");
        return true;
    }
    String response("Api Handler: ");
    response += path.c_str();
    server.send(200, "text/plain", response);
    return true;
}
