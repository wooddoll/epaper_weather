#ifndef _WIFI_TIME_H_
#define _WIFI_TIME_H_

#include <Arduino.h>


int StartWiFi(const char* ssid, const char* passwd);
void StopWiFi();
struct tm UpdateLocalTime();
void SetupTime(String Timezone);



typedef struct { // For current Day and Day 1, 2, 3, etc
  int      Dt;
  String   Period;
  String   Icon;
  String   Trend;
  String   Main0;
  String   Forecast0;
  String   Forecast1;
  String   Forecast2;
  String   Description;
  String   Time;
  String   Country;
  float    lat;
  float    lon;
  float    Temperature;
  float    Feelslike;
  float    Humidity;
  float    High;
  float    Low;
  float    Winddir;
  float    Windspeed;
  float    Rainfall;
  float    Snowfall;
  float    Pop;
  float    Pressure;
  int      Cloudcover;
  int      Visibility;
  int      Sunrise;
  int      Sunset;
  int      Timezone;
} Forecast_record_type;


class OpenWeather {
public:
    OpenWeather(String City, String API_KEY) : City_(City), API_KEY_(API_KEY) {}
    ~OpenWeather() {};

public:
    bool getWeather();

private:
    bool getWeatherData();    
    bool decodeWeather(String& json);
    bool getForecastData();
    bool decodeForecast(String& json);

public:
    static constexpr int maxReadings_ = 7;

    Forecast_record_type weatherData_;
    Forecast_record_type forecastData_[maxReadings_];

    const String City_;
    const String API_KEY_;
};

#endif//_WIFI_TIME_H_