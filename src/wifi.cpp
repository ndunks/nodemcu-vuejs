#include "header.h"


void wifi_apmode()
{
    // AP Mode
    char ssid[16] = {0};
    sprintf(ssid, "Wifi_%s", device_id);
    if (WiFi.softAP(ssid))
    {
        Serial.print("AP ");
        Serial.println(ssid);
        Serial.print("IP ");
        Serial.println(WiFi.softAPIP());
    }
    else
    {
        Serial.println("AP ERROR");
    }
}

void wifi_connect()
{
    uint8 counter = 0;
    Serial.printf("Connecting to: %s\n", WiFi.SSID().c_str());
    WiFi.begin();
    while (true)
    {
        Serial.printf("%d ", counter);
        delay(1000);
        if (WiFi.status() == WL_CONNECTED)
        {
            Serial.println("OK");
            break;
        }
        // Timeout 10 detik
        if (++counter == 10)
        {
            // Fail connect, fallback apmode
            Serial.println("TIMEOUT");
            WiFi.enableSTA(false);
            // Blink led indicate error
            ticker.attach_ms(1000, blink_error);
            wifi_apmode();
        }
    }
}

void wifi_begin()
{

    //Cek apakah sudah ada SSID tersimpan
    if (WiFi.SSID().length() > 0)
    {
        //Connect to saved
        wifi_connect();
    }
    else
    {
        Serial.println("No SSID saved, activating apmode");
        wifi_apmode();
    }
}

