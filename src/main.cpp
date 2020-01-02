#include <header.h>

Ticker ticker;
char device_id[9] = {0};

void setup()
{
  Serial.begin(115200);
  // Setup PIN
  pinMode(ledPin, OUTPUT);
  pinMode(btnPin, INPUT);
  digitalWrite(ledPin, HIGH);

  delay(600);

  // get chip id as device id
  uint32 num = system_get_chip_id();
  sprintf(device_id,
          "%02X%02X%02X%02X",
          (byte)(num >> 24),
          (byte)(num >> 16),
          (byte)(num >> 8),
          (byte)(num >> 0));

  Serial.printf("Device ID: %s\n", device_id);

  wifi_begin();
}

void loop()
{
}