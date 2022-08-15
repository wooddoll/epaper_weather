#include <Arduino.h>
#include <WiFi.h>

#include "credentials.h"
#include "serialCmd.h"
#include "weathertime.h"
#include "EPD_GDEW042T2.h"
#include "gray2bitmap.h"
#include "drawWeather.h"

#include "logo.h"

const char* Timezone = "KST-9";                        // Choose your time zone from: https://github.com/nayarsystems/posix_tz_db/blob/master/zones.csv 
constexpr long SleepDuration = 30; // Sleep time in minutes, aligned to the nearest minute boundary, so if 30 will always update at 00 or 30 past the hour
constexpr long fullRefreshTime = 2000;
Credentials credentials;

static const uint8_t EPD_BUSY = 14;  // to EPD BUSY
static const uint8_t EPD_CS   = 5;  // to EPD CS
static const uint8_t EPD_RST  = 12; // to EPD RST
static const uint8_t EPD_DC   = 27; // to EPD DC
static const uint8_t EPD_SCK  = 18; // to EPD CLK
static const uint8_t EPD_MISO = 19; // Master-In Slave-Out not used, as no data from display
static const uint8_t EPD_MOSI = 23; // to EPD DIN

EPD_GDEW042T2 display(EPD_SCK, EPD_MISO, EPD_MOSI, EPD_CS, EPD_DC, EPD_RST, EPD_BUSY);


void SetupWIFI() {
  int WifiFailCount = 0;
  while(true) {
    if(!credentials.SSID[WifiFailCount].isEmpty() && 
      StartWiFi(credentials.SSID[WifiFailCount].c_str(), credentials.passwd[WifiFailCount].c_str()) == WL_CONNECTED)
      break;

      ++WifiFailCount;
      if(WifiFailCount >= Credentials::maxSSIDs) {
        if(credentials.FailCount == 0) {
          credentials.FailCount = 1;
          credentials.write();
        }

        ESP.restart();
      }
  }
}

void InitialiseDisplay() {
  display.init_4Gray();
}

void BeginSleep() {
  display.sleep();
  Serial.println("\n");
  struct tm timeinfo = UpdateLocalTime();
  
  long SleepTimer = (SleepDuration * 60 - ((timeinfo.tm_min % SleepDuration) * 60 + timeinfo.tm_sec)); //Some ESP32 are too fast to maintain accurate time
  esp_sleep_enable_timer_wakeup((SleepTimer+20) * 1000000LL); // Added +20 seconnds to cover ESP32 RTC timer source inaccuracies
  Serial.print(F("Entering "));
  Serial.println(String(SleepTimer) + "-secs of sleep time");
  Serial.println(F("Starting deep-sleep period..."));
  esp_deep_sleep_start();      // Sleep for e.g. 30 minutes
}

void cmdLoop() {
  if(Serial.available() > 0) 
  {
    SerialCommand serialCmd(credentials);
    serialCmd.loop();
  }
}
void setup() {
  Serial.begin(115200);

  Serial.println();
  Serial.println(F("If you want to update WIFI SSID / PASSWORD, CITY name or OpenWeather API KEY,"));
  Serial.println(F("Send any charector using serial during boot process."));

  if(credentials.FailCount == 0) {
    InitialiseDisplay();
    display.grayDisplayPGM(gImage_4G_logo);
    cmdLoop();
  }

  SetupWIFI();
  cmdLoop();

  unsigned long StartTime = millis();

  //init and get the time
  SetupTime(Timezone);  

  OpenWeather weather(credentials.City, credentials.API_key);
  if(weather.getWeather())
  {
    Gray2bitMap bitmap;
    bitmap.setTo(WHITE);  

    DrawWeather draw(bitmap);
    draw.displayWeather(weather);
    
    unsigned long elleTime = millis() - StartTime;
    if(elleTime < fullRefreshTime)
      delay(fullRefreshTime - elleTime);
    display.grayDisplay(bitmap.buffer_);
  }

  if(credentials.FailCount != 0) {
    credentials.FailCount = 0;
    credentials.write();
  }

  cmdLoop();

  BeginSleep();
}

void loop() {
  // put your main code here, to run repeatedly:
}