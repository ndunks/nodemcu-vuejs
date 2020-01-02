#include "header.h"
// https://arduino-esp8266.readthedocs.io/en/latest/filesystem.html#flash-layout
/* static const char *const status_text[] = {
    "IDLE",
    "CONNECTING",
    "WRONG_PASSWORD",
    "NO_AP_FOUND",
    "CONNECT_FAIL",
    "GOT_IP"};
static const char *const mode_text[] = {
    "OFF", "STA", "AP", "AP_STA",
    //these two pseudo modes are experimental
    "SHUTDOWN", "", "", "", "RESUME"};
 */


station_config config;
char device_id[9] = {0};

void wifi_apmode()
{
    // AP Mode
    char ssid[16] = {0};
    sprintf(ssid, "Wifi_%s", device_id);
    if (WiFi.softAP(ssid, "12345678"))
    {
        Serial.print("AP ");
        Serial.println(ssid);
    }
    else
    {
        Serial.println("AP ERROR");
    }
}

void wifi_blink_error()
{
    digitalWrite(ledPin, !digitalRead(ledPin));
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
            ticker.attach_ms(500, wifi_blink_error);
            wifi_apmode();
        }
    }
}

void wifi_begin()
{
    // get chip id as device id
    uint32 num = system_get_chip_id();
    uint8 bytes[4] = {(byte)(num >> 24), (byte)(num >> 16), (byte)(num >> 8), (byte)num};
    sprintf(device_id, "%02X%02X%02X%02X", bytes[0], bytes[1], bytes[2], bytes[3]);
    //Cek apakah sudah ada SSID tersimpan
    if (WiFi.SSID().length() > 0)
    {
        //Connect to saved
        ticker.once_ms(100, wifi_connect);
    }
    else
    {
        Serial.println("No SSID saved, activating apmode");
        ticker.once_ms(100, wifi_apmode);
    }
}
