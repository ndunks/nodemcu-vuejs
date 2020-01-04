#pragma once
#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <Ticker.h>
#include <ESP8266WebServer.h>
#include <EEPROM.h>
#include <DNSServer.h>
#include <FS.h>

#define ledPin D4
#define btnPin D8 // RX GPIO 03
#define FLAG_IS_BOOTED 0x1
// Debug HTTPServer
#define DEBUG_ESP_HTTP_SERVER 1

extern Ticker ticker;
// Config that stored on EEPROM
struct Config
{
    // max 8 char, 9 is null
    uint8 flag;
    char password[9];
};

// main.c
extern char device_id[7];
extern Config *config;
extern IPAddress local_IP;
void blink_error();

//  server.cpp
void server_begin();
void server_loop();

// api_handler.cpp

class ApiHandler : public RequestHandler
{
public:
    ApiHandler(const char *uri = "/api")
        : prefix(uri)
    {
    }
    bool canHandle(HTTPMethod method, String uri);
    bool handle(ESP8266WebServer &server, HTTPMethod requestMethod, String requestUri);

protected:
    String prefix;
};

