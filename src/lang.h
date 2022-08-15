#ifndef _LOCALIZATON_H_
#define _LOCALIZATON_H_

#include <Arduino.h>

//Temperature - Humidity - Forecast
extern const char* TXT_FORECAST_VALUES;
extern const char* TXT_CONDITIONS;
extern const char* TXT_DAYS;
extern const char* TXT_TEMPERATURES;
extern const char* TXT_TEMPERATURE_C;
extern const char* TXT_TEMPERATURE_F;
extern const char* TXT_HUMIDITY_PERCENT;

// Pressure
extern const char* TXT_PRESSURE;
extern const char* TXT_PRESSURE_HPA;
extern const char* TXT_PRESSURE_IN;
extern const char* TXT_PRESSURE_STEADY;
extern const char* TXT_PRESSURE_RISING;
extern const char* TXT_PRESSURE_FALLING;

//RainFall / SnowFall
extern const char* TXT_RAINFALL_MM;
extern const char* TXT_RAINFALL_IN;
extern const char* TXT_SNOWFALL_MM;
extern const char* TXT_SNOWFALL_IN;
extern const char* TXT_PRECIPITATION_SOON;


//Sun
extern const char* TXT_SUNRISE;
extern const char* TXT_SUNSET;

//Moon
extern const char* TXT_MOON_NEW;
extern const char* TXT_MOON_WAXING_CRESCENT;
extern const char* TXT_MOON_FIRST_QUARTER;
extern const char* TXT_MOON_WAXING_GIBBOUS;
extern const char* TXT_MOON_FULL;
extern const char* TXT_MOON_WANING_GIBBOUS;
extern const char* TXT_MOON_THIRD_QUARTER;
extern const char* TXT_MOON_WANING_CRESCENT;

//Power / WiFi
extern const char* TXT_POWER;
extern const char* TXT_WIFI;
extern const char* TXT_UPDATED;


//Wind
extern const char* TXT_WIND_SPEED_DIRECTION;
extern const char* TXT_N;
extern const char* TXT_NNE;
extern const char* TXT_NE;
extern const char* TXT_ENE;
extern const char* TXT_E;
extern const char* TXT_ESE;
extern const char* TXT_SE;
extern const char* TXT_SSE;
extern const char* TXT_S;
extern const char* TXT_SSW;
extern const char* TXT_SW;
extern const char* TXT_WSW;
extern const char* TXT_W;
extern const char* TXT_WNW;
extern const char* TXT_NW;
extern const char* TXT_NNW;

//Day of the week
extern const char* weekday_D[];

//Month
extern const char* month_M[];

#endif//_LOCALIZATON_H_