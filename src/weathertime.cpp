#include "weathertime.h"
#include <Arduino.h>
#include <ArduinoJson.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include "utils.h"
#include "lang.h"


int StartWiFi(const char* ssid, const char* passwd) 
{
  Serial.print(F("\r\nConnecting to: ")); Serial.print(String(ssid)); 
  Serial.print(" ("); Serial.print(String(passwd)); Serial.println(")");

  WiFi.begin(ssid, passwd);

  unsigned long start = millis();
  uint8_t connectionStatus;
  bool AttemptConnection = true;

  while (AttemptConnection) {
    connectionStatus = WiFi.status();
    if (millis() > start + 10000) { // Wait 15-secs maximum
      AttemptConnection = false;
    }
    if (connectionStatus == WL_CONNECTED) {
      AttemptConnection = false;
    }
    Serial.print(".");
    delay(50);
  }

  if (connectionStatus == WL_CONNECTED) {
    Serial.println("\nWiFi connected at: " + WiFi.localIP().toString());
  }
  else 
    Serial.println(F("\nWiFi connection *** FAILED ***"));

  return connectionStatus;
}

void StopWiFi() {
  WiFi.disconnect();
  WiFi.mode(WIFI_OFF);
}

struct tm UpdateLocalTime() {  
  struct tm timeinfo;
  if (!getLocalTime(&timeinfo)) { // Wait for 5-sec for time to synchronise
    Serial.println(F("Failed to obtain time."));
    return timeinfo;
  }

  char time_output[30], day_output[30];
  snprintf(day_output, 30, "%i/%i/%i %s", (timeinfo.tm_year) + 1900, timeinfo.tm_mon + 1, timeinfo.tm_mday, weekday_D[timeinfo.tm_wday]);
  snprintf(time_output, 30, "%02i:%02i:%02i", timeinfo.tm_hour, timeinfo.tm_min, timeinfo.tm_sec);

  Serial.print(F("Current: "));
  Serial.println(String(day_output) + " " + String(time_output));
  
  return timeinfo;
}


void SetupTime(String Timezone) {
  onLED();
  configTime(0, 0, "pool.ntp.org");
  offLED();
  setenv("TZ", Timezone.c_str(), 1);  //setenv()adds the "TZ" variable to the environment with a value TimeZone, only used if set to 1, 0 means no change
  tzset(); // Set the TZ environment variable
  delay(500);
}

String getStream(HTTPClient &http) {
  String txt;

  WiFiClient* stream = http.getStreamPtr();
#define BUFF_SIZE   64
  uint8_t buff[BUFF_SIZE] = { 0 };

  // get length of document (is -1 when Server sends no Content-Length header)
  int len = http.getSize();
  while(http.connected() && (len > 0 || len == -1)) {
    size_t size = stream->available();
    if(size) {
      size = (size > (BUFF_SIZE-1)) ? (BUFF_SIZE-1) : size;
      buff[size] = 0;
      int c = stream->readBytes(buff, size);
      txt += String((char*)buff);

      if(len > 0) {
        len -= c;
      }
    }
    delay(1);
  }

  return txt;
}

bool OpenWeather::decodeWeather(String& json) {
  // allocate the JsonDocument
  DynamicJsonDocument doc(2 * 1024);
  // Deserialize the JSON document
  DeserializationError error = deserializeJson(doc, json);
  // Test if parsing succeeds.
  if (error) {
    Serial.print(F("deserializeJson() failed: "));
    Serial.println(error.c_str());
    return false;
  }
  // convert it to a JsonObject
  Serial.println(" Decoding weather data");

  // All Serial.println statements are for diagnostic purposes and not required, remove if not needed
  weatherData_.lon         = doc["coord"]["lon"].as<float>();                    Serial.println(" Lon: "+String(weatherData_.lon));
  weatherData_.lat         = doc["coord"]["lat"].as<float>();                    Serial.println(" Lat: "+String(weatherData_.lat));
  weatherData_.Main0       = doc["weather"][0]["main"].as<const char*>();        Serial.println("Main: "+String(weatherData_.Main0));
  weatherData_.Forecast0   = doc["weather"][0]["description"].as<const char*>(); Serial.println("For0: "+String(weatherData_.Forecast0));
  weatherData_.Forecast1   = doc["weather"][1]["description"].as<const char*>(); Serial.println("For1: "+String(weatherData_.Forecast1));
  weatherData_.Forecast2   = doc["weather"][2]["description"].as<const char*>(); Serial.println("For2: "+String(weatherData_.Forecast2));
  weatherData_.Icon        = doc["weather"][0]["icon"].as<const char*>();        Serial.println("Icon: "+String(weatherData_.Icon));
  weatherData_.Temperature = doc["main"]["temp"].as<float>();                    Serial.println("Temp: "+String(weatherData_.Temperature));
  weatherData_.Pressure    = doc["main"]["pressure"].as<float>();                Serial.println("Pres: "+String(weatherData_.Pressure));
  weatherData_.Humidity    = doc["main"]["humidity"].as<float>();                Serial.println("Humi: "+String(weatherData_.Humidity));
  weatherData_.Low         = doc["main"]["temp_min"].as<float>();                Serial.println("TLow: "+String(weatherData_.Low));
  weatherData_.High        = doc["main"]["temp_max"].as<float>();                Serial.println("THig: "+String(weatherData_.High));
  weatherData_.Windspeed   = doc["wind"]["speed"].as<float>();                   Serial.println("WSpd: "+String(weatherData_.Windspeed));
  weatherData_.Winddir     = doc["wind"]["deg"].as<float>();                     Serial.println("WDir: "+String(weatherData_.Winddir));
  weatherData_.Cloudcover  = doc["clouds"]["all"].as<int>();                     Serial.println("CCov: "+String(weatherData_.Cloudcover)); // in % of cloud cover
  weatherData_.Visibility  = doc["visibility"].as<int>();                        Serial.println("Visi: "+String(weatherData_.Visibility)); // in metres
  weatherData_.Rainfall    = doc["rain"]["1h"].as<float>();                      Serial.println("Rain: "+String(weatherData_.Rainfall));
  weatherData_.Snowfall    = doc["snow"]["1h"].as<float>();                      Serial.println("Snow: "+String(weatherData_.Snowfall));
  weatherData_.Country     = doc["sys"]["country"].as<const char*>();            Serial.println("Ctry: "+String(weatherData_.Country));
  weatherData_.Sunrise     = doc["sys"]["sunrise"].as<int>();                    Serial.println("SRis: "+String(weatherData_.Sunrise));
  weatherData_.Sunset      = doc["sys"]["sunset"].as<int>();                     Serial.println("SSet: "+String(weatherData_.Sunset));
  weatherData_.Timezone    = doc["timezone"].as<int>();                          Serial.println("TZon: "+String(weatherData_.Timezone));
  
  return true;
}

bool OpenWeather::getWeatherData() {
  HTTPClient http;
  String url = String(F("http://api.openweathermap.org/data/2.5/weather?q="));
  url += City_ + String(F(",KR&APPID=")) + API_KEY_ + String(F("&mode=json&units=metric&lang=kr"));
  Serial.println(url);

  onLED();
  http.begin(url);
  int httpCode = http.GET();
  offLED();
  if(httpCode == HTTP_CODE_OK) {
    onLED();
    String txt = getStream(http);
    http.end();
    Serial.println("json size: " + String(txt.length()));
    //Serial.println(txt);
    offLED();

    //writeSPIFFS("weatherSample.json", txt);

    return decodeWeather(txt);
  }

  Serial.print(F("connection failed, error:"));
  Serial.println(http.errorToString(httpCode));

  http.end();
  return false;
}

bool OpenWeather::decodeForecast(String& json) {
  // allocate the JsonDocument
  DynamicJsonDocument doc(32 * 1024);
  // Deserialize the JSON document
  Serial.println(F("deserializeJson..."));
  
  DeserializationError error = deserializeJson(doc, json);
  // Test if parsing succeeds.
  if (error) {
    Serial.print(F("deserializeJson() failed: "));
    Serial.println(error.c_str());
    return false;
  }
  // convert it to a JsonObject
  JsonObject root = doc.as<JsonObject>();
  Serial.println(F(" Decoding forecast data"));
  Serial.print(F("Receiving Forecast period - "));

  JsonArray list = root["list"];
  for (byte r = 0; r < list.size(); r++) {
    Serial.println("\nPeriod-" + String(r) + "--------------");
    forecastData_[r].Dt          = list[r]["dt"].as<int>();                                Serial.println("DTim: "+String(forecastData_[r].Dt));
    forecastData_[r].Temperature = list[r]["main"]["temp"].as<float>();                    Serial.println("Temp: "+String(forecastData_[r].Temperature));
    forecastData_[r].Low         = list[r]["main"]["temp_min"].as<float>();                Serial.println("TLow: "+String(forecastData_[r].Low));
    forecastData_[r].High        = list[r]["main"]["temp_max"].as<float>();                Serial.println("THig: "+String(forecastData_[r].High));
    forecastData_[r].Pressure    = list[r]["main"]["pressure"].as<float>();                Serial.println("Pres: "+String(forecastData_[r].Pressure));
    forecastData_[r].Humidity    = list[r]["main"]["humidity"].as<float>();                Serial.println("Humi: "+String(forecastData_[r].Humidity));
    forecastData_[r].Forecast0   = list[r]["weather"][0]["main"].as<const char*>();        Serial.println("For0: "+String(forecastData_[r].Forecast0));
    forecastData_[r].Forecast1   = list[r]["weather"][1]["main"].as<const char*>();        Serial.println("For1: "+String(forecastData_[r].Forecast1));
    forecastData_[r].Forecast2   = list[r]["weather"][2]["main"].as<const char*>();        Serial.println("For2: "+String(forecastData_[r].Forecast2));
    forecastData_[r].Icon        = list[r]["weather"][0]["icon"].as<const char*>();        Serial.println("Icon: "+String(forecastData_[r].Icon));
    forecastData_[r].Description = list[r]["weather"][0]["description"].as<const char*>(); Serial.println("Desc: "+String(forecastData_[r].Description));
    forecastData_[r].Cloudcover  = list[r]["clouds"]["all"].as<int>();                     Serial.println("CCov: "+String(forecastData_[r].Cloudcover)); // in % of cloud cover
    forecastData_[r].Windspeed   = list[r]["wind"]["speed"].as<float>();                   Serial.println("WSpd: "+String(forecastData_[r].Windspeed));
    forecastData_[r].Winddir     = list[r]["wind"]["deg"].as<float>();                     Serial.println("WDir: "+String(forecastData_[r].Winddir));
    forecastData_[r].Rainfall    = list[r]["rain"]["3h"].as<float>();                      Serial.println("Rain: "+String(forecastData_[r].Rainfall));
    forecastData_[r].Snowfall    = list[r]["snow"]["3h"].as<float>();                      Serial.println("Snow: "+String(forecastData_[r].Snowfall));
    forecastData_[r].Pop         = list[r]["pop"].as<float>();                             Serial.println("Pop:  "+String(forecastData_[r].Pop));
    forecastData_[r].Period      = list[r]["dt_txt"].as<const char*>();                    Serial.println("Peri: "+String(forecastData_[r].Period));
  }
  
  return true;
}



bool OpenWeather::getForecastData() {
  HTTPClient http;
  String url = String(F("http://api.openweathermap.org/data/2.5/forecast?q="));
  url += City_ + String(F(",KR&APPID=")) + API_KEY_ + String(F("&mode=json&units=metric&lang=kr&cnt=")) + String(maxReadings_);;
  Serial.println(url);

  onLED();
  http.begin(url);
  int httpCode = http.GET();
  offLED();
  if(httpCode == HTTP_CODE_OK) {
    onLED();
    String txt = getStream(http);
    http.end();

    Serial.println("json size: " + String(txt.length()));
    //Serial.println(txt);
    offLED();

    //writeSPIFFS("forecatSample.json", txt);

    return decodeForecast(txt);
  }

  Serial.print(F("connection failed, error:"));
  Serial.println(http.errorToString(httpCode));
  http.end();

  return false;
}

bool OpenWeather::getWeather()
{
  byte Attempts = 1;
  bool reciveWeather = false, reciveForecast = false;

  while ((reciveWeather == false || reciveForecast == false) && Attempts <= 2) { // Try up-to 2 time for Weather and Forecast data
    if (reciveWeather == false) 
      reciveWeather = getWeatherData();
    if (reciveForecast == false) 
      reciveForecast = getForecastData();
    Attempts++;
  }

  //if(reciveWeather && reciveForecast) {
  //  for(int i=0; i<maxReadings_; ++i)
  //    forecastData_[i].Dt += weatherData_.Timezone;
  //}

  return reciveWeather && reciveForecast;
}