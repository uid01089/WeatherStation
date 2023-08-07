#include <Arduino.h>
#include <KStandardCore.h>
#include <KBlink.h>
#include <Weatherstation.h>

#include "incredential.h"

KStandardCore kStandardCore;
KBlink kblink;

Weatherstation weatherstation(&kStandardCore);




void setup()
{

  Serial.begin(9600); // Baudrate: 9600

  kStandardCore.setup("house/garden/weatherstation", SSID, PSK, MQTT_BROKER, 1883);

  weatherstation.setup();
  Serial.print("Weatherstation start done\n");

  kblink.setup(kStandardCore.getKSchedule(), 100, 3000);
}

void loop()
{
  kStandardCore.loop();
  kblink.loop();
  weatherstation.loop();
}
