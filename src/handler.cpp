#include "header.h"


bool Handler::canHandle(HTTPMethod method, String path)
{
    if (method == HTTP_OPTIONS)
    {
        return true;
    }
    // no handle without subPath, eg: /api/
    if (path.length() <= prefix.length())
    {
        return false;
    }
    return path.startsWith(prefix);
}

bool Handler::handle(ESP8266WebServer &server, HTTPMethod method, String path)
{
    Serial.printf("Handler: %s\n", path.c_str());
    server.sendHeader("Access-Control-Allow-Origin", "*");
    if (method == HTTP_OPTIONS)
    {
        server.sendHeader("Access-Control-Allow-Methods", "GET,POST");
        server.sendHeader("Access-Control-Allow-Headers", "*");
        // 60 * 60 * 60 * 30 = 6480000
        server.sendHeader("Access-Control-Max-Age", "6480000");
        server.send(200, "text/plain");
        return true;
    }
    String response;
    String subPath = path.substring(prefix.length());
    Serial.printf("Api Check: %s\n", subPath.c_str());
    Controller *matched = routes;

    while (matched < routeEnd)
    {
        // Serial.printf("  > %d %d %d %d\n", !matched, (uint)matched, sizeof(matched), sizeof(matched->function));
        // Serial.printf("    %s = %d\n", matched->path, subPath.equals(matched->path));
        if (subPath.equals(matched->path))
        {
            if (matched->mustLogin && !server_guard())
            {
                // blocked
                return true;
            }
            matched->function(response, method);
            if (response.length())
            {
                server.send(200, "text/plain", response);
            } // no response mean handled directly
            response.clear();
            return true;
        }
        matched++;
    }

    Serial.printf("No Api handle it\n");
    return false;
}
