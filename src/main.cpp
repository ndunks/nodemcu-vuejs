#include <EEPROM.h>
#include <header.h>

Ticker ticker;
char device_id[9] = {0};
Config *config;

void dump_config()
{
  Serial.printf("----Config----\n");
  Serial.printf("boot: %d\nPassword: %s\n", config->booted, config->password);
  Serial.println("---------------");
}

void blink_error()
{
    digitalWrite(ledPin, !digitalRead(ledPin));
}

void setup()
{
  Serial.begin(115200);
  // Setup PIN
  pinMode(ledPin, OUTPUT);
  pinMode(btnPin, INPUT);
  digitalWrite(ledPin, HIGH);

  // get chip id as device id
  uint32 num = system_get_chip_id();
  sprintf(device_id,
          "%02X%02X%02X%02X",
          (byte)(num >> 24),
          (byte)(num >> 16),
          (byte)(num >> 8),
          (byte)(num >> 0));

  Serial.printf("Device ID: %s\n", device_id);

  // Setup EEPROM
  EEPROM.begin(sizeof(Config));
  config = (Config *)EEPROM.getDataPtr();
  delay(1000);

  if (!config->booted)
  {
    // First time boot
    config->booted = true;
    strncpy(config->password, "admin", sizeof(config->password) - 1);
    if (EEPROM.commit())
    {
      Serial.println("initial config UPDATED");
    }
    else
    {
      Serial.println("!!!CANNOT Write EEPROM!!!");
      dump_config();
      // fast blink
      ticker.attach_ms(200, blink_error);
      return;
    }
  }

  dump_config();
  wifi_begin();
  server_begin();
}

void loop()
{
  // sensor runtime
  server_loop();
}