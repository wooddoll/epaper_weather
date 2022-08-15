#include <Arduino.h>
#include "lang.h"

//Temperature - Humidity - Forecast
const char* TXT_FORECAST_VALUES  = "일기예보(3일)";
const char* TXT_CONDITIONS       = "Conditions";
const char* TXT_DAYS             = "(Days)";
const char* TXT_TEMPERATURES     = "Temperature";
const char* TXT_TEMPERATURE_C    = "기온(\'C)";
const char* TXT_HUMIDITY_PERCENT = "습도(%)";

// Pressure
const char* TXT_PRESSURE         = "Pressure";
const char* TXT_PRESSURE_HPA     = "기압(hPa)";
const char* TXT_PRESSURE_STEADY  = "Steady";
const char* TXT_PRESSURE_RISING  = "Rising";
const char* TXT_PRESSURE_FALLING = "Falling";

//RainFall / SnowFall
const char* TXT_RAINFALL_MM = "강우량(mm)";
const char* TXT_SNOWFALL_MM = "강설량(mm)";
const char* TXT_PRECIPITATION_SOON = "Prec.";


//Sun
const char* TXT_SUNRISE  = "일출";
const char* TXT_SUNSET   = "일몰";

//Moon
const char* TXT_MOON_NEW             = "신월";
const char* TXT_MOON_WAXING_CRESCENT = "초승달";
const char* TXT_MOON_FIRST_QUARTER   = "상현달";
const char* TXT_MOON_WAXING_GIBBOUS  = "차가는달";
const char* TXT_MOON_FULL            = "보름달";
const char* TXT_MOON_WANING_GIBBOUS  = "기울어가는달";
const char* TXT_MOON_THIRD_QUARTER   = "하현달";
const char* TXT_MOON_WANING_CRESCENT = "그믐달";

//Power / WiFi
const char* TXT_POWER  = "Power";
const char* TXT_WIFI   = "WiFi";
const char* TXT_UPDATED = "Updated:";


//Wind
const char* TXT_WIND_SPEED_DIRECTION = "Wind Speed/Direction";
const char* TXT_N   = "N";
const char* TXT_NNE = "NNE";
const char* TXT_NE  = "NE";
const char* TXT_ENE = "ENE";
const char* TXT_E   = "E";
const char* TXT_ESE = "ESE";
const char* TXT_SE  = "SE";
const char* TXT_SSE = "SSE";
const char* TXT_S   = "S";
const char* TXT_SSW = "SSW";
const char* TXT_SW  = "SW";
const char* TXT_WSW = "WSW";
const char* TXT_W   = "W";
const char* TXT_WNW = "WNW";
const char* TXT_NW  = "NW";
const char* TXT_NNW = "NNW";

//Day of the week
const char* weekday_D[] = { "일요일", "월요일", "화요일", "수요일", "목요알", "금요일", "토요일" };
