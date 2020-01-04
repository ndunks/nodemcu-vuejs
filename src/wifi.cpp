#include "header.h"
void wifi_smartconfig_watcher()
{

    if (WiFi.smartConfigDone())
    {
        Serial.println("WiFi SmartConfig Done");
        ticker.detach();
        digitalWrite(ledPin, HIGH);
    }
    else if (WiFi.isConnected())
    {
        WiFi.stopSmartConfig();
        Serial.println("WiFi Connected, SmartConfig canceled.");
        ticker.detach();
        digitalWrite(ledPin, HIGH);
    }
    else
    {
        digitalWrite(ledPin, !digitalRead(ledPin));
    }
}
void wifi_config_mode()
{
    // Custom AP Name by devide ID
    String ssid("Wifi-");
    ssid += device_id;
    // Default is equal with hostname
    if (WiFi.softAP(ssid))
    {
        Serial.print("AP ");
        Serial.println(ssid.c_str());
        Serial.print("IP ");
        Serial.println(WiFi.softAPIP());
    }
    else
    {
        Serial.printf("Fail set AP Name to: %s", ssid.c_str());
    }
    ssid.clear();
    if (ticker.active())
    {
        ticker.detach();
    }
    WiFi.beginSmartConfig();
    Serial.println("SmartConfig Started");
    ticker.attach(1.0f, wifi_smartconfig_watcher);
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
            //WiFi.enableAP(false);
            Serial.println("OK");
            break;
        }
        // Timeout 20 detik
        if (++counter == 20)
        {
            // Fail connect, fallback apmode
            Serial.println("TIMEOUT");
            WiFi.enableSTA(false);
            // Blink led indicate error
            ticker.attach_ms(300, blink_error);
            delay(1000);
            wifi_config_mode();
            break;
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
        wifi_config_mode();
    }
}
