#include <header.h>

Ticker ticker;

void setup()
{

  Serial.begin(115200);
  delay(1000);
  wifi_begin();
  pinMode(ledPin, OUTPUT);
  pinMode(btnPin, INPUT);
  digitalWrite(ledPin, HIGH);

  Serial.print("Device ID: ");
  Serial.println(device_id);
}

void loop()
{
  
}