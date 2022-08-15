#include <Arduino.h>
#include <SPIFFS.h>

int julianDate(int d, int m, int y) {
  int mm, yy, k1, k2, k3, j;
  yy = y - (int)((12 - m) / 10);
  mm = m + 9;
  if (mm >= 12) 
    mm = mm - 12;
  k1 = (int)(365.25 * (yy + 4712));
  k2 = (int)(30.6001 * mm + 0.5);
  k3 = (int)((int)((yy / 100) + 49) * 0.75) - 38;
  // 'j' for dates in Julian calendar:
  j = k1 + k2 + d + 59 + 1;
  if (j > 2299160) 
    j = j - k3; // 'j' is the Julian date at 12h UT (Universal Time) For Gregorian calendar:
  return j;
}

double normalizedMoonPhase(int d, int m, int y) {
  int j = julianDate(d, m, y);
  //Calculate the approximate phase of the moon
  double Phase = (j + 4.867) / 29.53059;
  return (Phase - (int) Phase);
}


#define LED 2
static bool initLED = false;
void onLED() {
    
    if(!initLED) {
        pinMode(LED, OUTPUT);
        initLED = true;
    }
    digitalWrite(LED, HIGH);
}

void offLED() {
    if(!initLED) {
        pinMode(LED, OUTPUT);
        initLED = true;
    }
    digitalWrite(LED, LOW);
}


String convertUnixTime(int unix_time) {
  // Returns either '21:12  ' or ' 09:12pm' depending on Units mode
  time_t tm = unix_time;
  struct tm *now_tm = localtime(&tm);
  char time_output[30];
  snprintf(time_output, 30, "%02i:%02i:%02i", now_tm->tm_hour, now_tm->tm_min, now_tm->tm_sec);

  return time_output;
}

String titleCase(String text) {
  if (text.length() > 0) {
    String temp_text = text.substring(0,1);
    temp_text.toUpperCase();
    return temp_text + text.substring(1); // Title-case the string
  }
  else return text;
}


void writeSPIFFS(String path, String data) {
  File fp = SPIFFS.open(path, "w", true);
  if (!fp) {  // can not open file
      Serial.println(F("# ERROR: can not open file"));
      return;
  }
  fp.print(data);
  fp.close();
}

String readSPIFFS(String path) {
  File fp = SPIFFS.open(path);
    if (!fp) {  // can not open file
        Serial.println(F("# ERROR: can not open file"));
        return String();
    }
    String data = fp.readString();
    fp.close();

    return data;
}