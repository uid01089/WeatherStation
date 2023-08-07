#include "Weatherstation.h"

Weatherstation::Weatherstation(KStandardCore *kStandardCore)
{
    this->kStandardCore = kStandardCore;
}

Weatherstation::~Weatherstation()
{
}

void Weatherstation::setup()
{
    weatherSensor.begin();

    kStandardCore->getKSchedule()->schedule(std::bind(&Weatherstation::clcWeatherstation_50ms, this), 100);


}

void Weatherstation::loop()
{
}

void Weatherstation::clcWeatherstation_50ms()
{
    // This example uses only a single slot in the sensor data array
    int const i = 0;

    // Clear all sensor data
    weatherSensor.clearSlots();

    // Tries to receive radio message (non-blocking) and to decode it.
    // Timeout occurs after a small multiple of expected time-on-air.
    int decode_status = weatherSensor.getMessage();

    if (decode_status == DECODE_OK)
    {

        Serial.printf("Id: [%8X] Typ: [%X] Battery: [%s] ",
                      weatherSensor.sensor[i].sensor_id,
                      weatherSensor.sensor[i].s_type,
                      weatherSensor.sensor[i].battery_ok ? "OK " : "Low");

        kStandardCore->getKMqtt()->publish("/" + kStandardCore->getTopicPathWithoutLeadingSlash() + "/sensor_id", String(weatherSensor.sensor[i].sensor_id).c_str());
        kStandardCore->getKMqtt()->publish("/" + kStandardCore->getTopicPathWithoutLeadingSlash() + "/s_type", String(weatherSensor.sensor[i].s_type).c_str());
        kStandardCore->getKMqtt()->publish("/" + kStandardCore->getTopicPathWithoutLeadingSlash() + "/battery_ok", String(weatherSensor.sensor[i].battery_ok).c_str());

        if (weatherSensor.sensor[i].temp_ok)
        {
            Serial.printf("Temp: [%5.1fC] ", weatherSensor.sensor[i].temp_c);
            kStandardCore->getKMqtt()->publish("/" + kStandardCore->getTopicPathWithoutLeadingSlash() + "/temp_c", String(weatherSensor.sensor[i].temp_c).c_str());
        }
        else
        {
            Serial.printf("Temp: [---.-C] ");
        }
        if (weatherSensor.sensor[i].humidity_ok)
        {
            Serial.printf("Hum: [%3d%%] ", weatherSensor.sensor[i].humidity);
            kStandardCore->getKMqtt()->publish("/" + kStandardCore->getTopicPathWithoutLeadingSlash() + "/humidity", String(weatherSensor.sensor[i].humidity).c_str());
        }
        else
        {
            Serial.printf("Hum: [---%%] ");
        }
        if (weatherSensor.sensor[i].wind_ok)
        {
            Serial.printf("Wind max: [%4.1fm/s] Wind avg: [%4.1fm/s] Wind dir: [%5.1fdeg] ",
                          weatherSensor.sensor[i].wind_gust_meter_sec,
                          weatherSensor.sensor[i].wind_avg_meter_sec,
                          weatherSensor.sensor[i].wind_direction_deg);
            kStandardCore->getKMqtt()->publish("/" + kStandardCore->getTopicPathWithoutLeadingSlash() + "/wind_gust_meter_sec", String(weatherSensor.sensor[i].wind_gust_meter_sec).c_str());
            kStandardCore->getKMqtt()->publish("/" + kStandardCore->getTopicPathWithoutLeadingSlash() + "/wind_avg_meter_sec", String(weatherSensor.sensor[i].wind_avg_meter_sec).c_str());
            kStandardCore->getKMqtt()->publish("/" + kStandardCore->getTopicPathWithoutLeadingSlash() + "/wind_direction_deg", String(weatherSensor.sensor[i].wind_direction_deg).c_str());
        }
        else
        {
            Serial.printf("Wind max: [--.-m/s] Wind avg: [--.-m/s] Wind dir: [---.-deg] ");
        }
        if (weatherSensor.sensor[i].rain_ok)
        {
            Serial.printf("Rain: [%7.1fmm] ", weatherSensor.sensor[i].rain_mm);
            kStandardCore->getKMqtt()->publish("/" + kStandardCore->getTopicPathWithoutLeadingSlash() + "/rain_mm", String(weatherSensor.sensor[i].rain_mm).c_str());
        }
        else
        {
            Serial.printf("Rain: [-----.-mm] ");
        }

        Serial.printf("RSSI: [%5.1fdBm]\n", weatherSensor.sensor[i].rssi);
        kStandardCore->getKMqtt()->publish("/" + kStandardCore->getTopicPathWithoutLeadingSlash() + "/rssi", String(weatherSensor.sensor[i].rssi).c_str());

        // Wait some minutes longer
        kStandardCore->getKSchedule()->schedule(std::bind(&Weatherstation::clcWeatherstation_50ms, this), 1000);

    }
    else
    {
        // Try again
        kStandardCore->getKSchedule()->schedule(std::bind(&Weatherstation::clcWeatherstation_50ms, this), 100);

    }
    
}
