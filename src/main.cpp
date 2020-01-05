#include <EEPROM.h>
#include <header.h>

Ticker ticker;
char device_id[7] = {0};
Config *config;

// Local IP on AP Mode
IPAddress local_IP(192, 168, 4, 1);

void dump_config()
{
  Serial.printf("----Config----\n");
  Serial.printf("Flag: %d\nWeb Password: %s\n", config->flag, config->password);
  Serial.printf("WiFi Auto Connect: %d\n", WiFi.getAutoConnect());
  Serial.println("---------------");
}

void blink_error()
{
  digitalWrite(ledPin, !digitalRead(ledPin));
}
/* 
void smartconfig_watcher()
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

void wifisetupmode()
{
  WiFi.enableAP(true);
  WiFi.beginSmartConfig();
  Serial.println("SmartConfig Started");
  ticker.attach(1.0f, smartconfig_watcher);
} */

void firstboot()
{
  String ssid("smart-");
  ssid += device_id;

  Serial.println("--- FIRST BOOT ---");
  Serial.printf("Device ID: %s\n", device_id);
  WiFi.hostname(ssid.c_str());

  if (!WiFi.softAPConfig(local_IP, local_IP, IPAddress(255, 255, 255, 0)))
  {
    Serial.println("Error: CANNOT Setup softAP IP");
    ticker.attach_ms(500, blink_error);
  }
  if (!WiFi.softAP(ssid))
  {
    Serial.println("Error: CANNOT Setup softAP SSID");
    ticker.attach_ms(500, blink_error);
  }

  WiFi.mode(WIFI_AP_STA);
  WiFi.setAutoConnect(false);

  if (config->password[0] == 0 || config->password[0] == 0xff)
  {
    // Default config
    Serial.println("Set default web password to \"admin\"");
    strncpy(config->password, "admin", sizeof(config->password));
    // make sure null terminated
    config->password[8] = 0;
  }
  config->flag = FLAG_IS_BOOTED;
  if (EEPROM.commit())
  {
    Serial.println("initial config UPDATED");
  }
  else
  {
    Serial.println("!!!CANNOT Write EEPROM!!!");
    // fast blink
    ticker.attach_ms(200, blink_error);
  }
  dump_config();
}

void setup()
{
  Serial.begin(115200);
  // Setup PIN
  pinMode(ledPin, OUTPUT);
  pinMode(btnPin, INPUT);
  digitalWrite(ledPin, HIGH);

  // get chip id as device id, leave first byte, is zero
  uint32 num = system_get_chip_id();
  sprintf(device_id,
          "%02X%02X%02X",
          (byte)(num >> 16),
          (byte)(num >> 8),
          (byte)(num >> 0));

  // Setup EEPROM
  EEPROM.begin(sizeof(Config));
  config = (Config *)EEPROM.getDataPtr();

  // After erase flash, whole flash contain 0xff
  if (config->flag != FLAG_IS_BOOTED)
  {
    delay(1000);
    firstboot();
  }
  else
  {
    dump_config();
  }

  if (WiFi.getAutoConnect())
  {
    //wait connected
    Serial.print("Wifi auto connect.. ");
    int8 status = WiFi.waitForConnectResult(15000);
    if (status == WL_CONNECTED)
    {
      Serial.println("CONNECTED");
    }
    else
    {
      Serial.printf("FAIL %d\n", status);
    }
  }
  server_begin();
}

void loop()
{
  // sensor runtime
  server_loop();
}