#pragma once
#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <Ticker.h>

#define ledPin D4
#define btnPin D8  // RX GPIO 03
#define FLAG_IS_BOOTED 0x1


extern Ticker ticker;
// Config that stored on EEPROM
struct Config {
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
    

