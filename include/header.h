#pragma once
#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <Ticker.h>

#define ledPin D4
#define btnPin D8  // RX GPIO 03

extern Ticker ticker;

// main.c
extern char device_id[9];
//  wifi.c
void wifi_begin();
    

