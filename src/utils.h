#ifndef _UTILS_H_
#define _UTILS_H_

#include <Arduino.h>

#ifndef MAX
#define MAX(a, b)   ((a) > (b) ? (a) : (b))
#endif//MAX

#ifndef MIN
#define MIN(a, b)   ((a) < (b) ? (a) : (b))
#endif//MAX

String convertUnixTime(int unix_time);
double normalizedMoonPhase(int d, int m, int y);
String titleCase(String text);

void onLED();
void offLED();

void writeSPIFFS(String path, String data);
String readSPIFFS(String path);

#endif//_UTILS_H_