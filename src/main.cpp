#include <EEPROM.h>
#include <header.h>

Ticker ticker;
char device_id[7] = {0};
Config *config;

int btnState = HIGH;
ulong btnPressedAt = 0;

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
  //pinMode(btnPin, INPUT);
  pinMode(btnPin, INPUT_PULLUP);

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

  // uint32_t wifiSector = (((uint32_t)0x405FD000 - 0x40200000) / SPI_FLASH_SEC_SIZE);
  // Serial.printf("SPI_FLASH_SEC_SIZE %d\n", SPI_FLASH_SEC_SIZE);
  // Serial.printf("EEPROM.length() %d\n", EEPROM.length());
  // Serial.printf("WifiSector %d %08X\n", wifiSector, wifiSector);
  
}
void resetConfigurationAndData()
{
  uint32 startSector = 0x003fb000;
  uint32 sector = 0x0;
  Serial.println("--- DO ERASE CONFIG ----");
  //DELETE 5 Sector
  for (int i = 0; i < 5; i++)
  {
    sector = startSector + SPI_FLASH_SEC_SIZE * i;
    Serial.printf("%08X ... ", sector);
    if (ESP.flashEraseSector(sector))
    {
      Serial.println("OK");
    }
    else
    {
      Serial.println("FAILED");
    }
  }
  Serial.println("REBOOTING...\n");

  /*
  for (uint i = 0; i < EEPROM.length(); i++)
  {
    Serial.printf(" %02X %c", EEPROM.read(i), (char)EEPROM.read(i));
    //EEPROM.write(i, -1);
  }
  //EEPROM.commit();
  Serial.println();
  //SPI_FLASH_SEC_SIZE
  uint configStart = 0x003fb000;
  int configSize = 1024; */
  // just used area, all size is 1024 * 4
  /* uint8_t * buffer;
  for (int i = 0; i < configSize; i++)
  {
    Serial.printf("%02X %c", buffer[i], (char)buffer[i]);
    if (i % 16 == 0)
    {
      Serial.println();
    }
  } */
}
void loop()
{
  // Detect reset action, press for 5 seconds
  if (digitalRead(btnPin) != btnState)
  {
    btnState = digitalRead(btnPin);
    if (btnState == HIGH)
    {
      // released
      Serial.printf("btn released after %lu\n", (millis() - btnPressedAt) / 1000UL);
      btnPressedAt = 0;
    }
    else
    {
      // begin pressed
      Serial.println("PRESSED");
      btnPressedAt = millis();
    }
  }
  else if (btnPressedAt > 0 && (millis() - btnPressedAt) >= 5000)
  {
    Serial.println("Do Reset!!");
    btnPressedAt = 0;
    resetConfigurationAndData();
  }
  // sensor runtime
  server_loop();
}