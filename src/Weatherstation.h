#ifndef __WEATHERSTATION_H__
#define __WEATHERSTATION_H__

#include <KStandardCore.h>
#include <Arduino.h>
#include "WeatherSensorCfg.h"
#include "WeatherSensor.h"



class Weatherstation
{
private:
    KStandardCore *kStandardCore;
    WeatherSensor weatherSensor;

    void clcWeatherstation_50ms();

 
public:
    Weatherstation(KStandardCore *kStandardCore);
    ~Weatherstation();

    void setup();
    void loop();

};

#endif // __WEATHERSTATION_H__