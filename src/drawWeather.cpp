#include "drawWeather.h"
#include "lang.h"
#include "utils.h"




static const char TXT_CELSIUS[3] = { 9, 67, 0 };
static const char TXT_DOT[3] = { 9, 0 };
static const String STR_CELSIUS(TXT_CELSIUS);
static const String STR_DOT(TXT_DOT);

String DrawWeather::moonPhase(int d, int m, int y) {
    int c, e;
    double jd;
    int b;
    if (m < 3) {
        y--;
        m += 12;
    }
    ++m;
    c = 365.25 * y;
    e = 30.6  * m;
    jd = c + e + d - 694039.09;     /* jd is total days elapsed */
    jd /= 29.53059;                  /* divide by the moon cycle (29.53 days) */
    b = jd;                        /* int(jd) -> b, take integer part of jd */
    jd -= b;                         /* subtract integer part to leave fractional part of original jd */
    b = jd * 8 + 0.5;              /* scale fraction from 0-8 and round by adding 0.5 */
    b = b & 7;                     /* 0 and 8 are the same phase so modulo 8 for 0 */

    if (hemisphere_ == Hemisphere::SOUTH)
        b = 7 - b;

    if (b == 0) return TXT_MOON_NEW;              // New;              0%  illuminated
    if (b == 1) return TXT_MOON_WAXING_CRESCENT;  // Waxing crescent; 25%  illuminated
    if (b == 2) return TXT_MOON_FIRST_QUARTER;    // First quarter;   50%  illuminated
    if (b == 3) return TXT_MOON_WAXING_GIBBOUS;   // Waxing gibbous;  75%  illuminated
    if (b == 4) return TXT_MOON_FULL;             // Full;            100% illuminated
    if (b == 5) return TXT_MOON_WANING_GIBBOUS;   // Waning gibbous;  75%  illuminated
    if (b == 6) return TXT_MOON_THIRD_QUARTER;    // Third quarter;   50%  illuminated
    if (b == 7) return TXT_MOON_WANING_CRESCENT;  // Waning crescent; 25%  illuminated
    return "";
}

void DrawWeather::drawMoon(int x, int y, int dd, int mm, int yy) {
    const int diameter = 38;
    double Phase = normalizedMoonPhase(dd, mm, yy);
    
    if (hemisphere_ == Hemisphere::SOUTH)
        Phase = 1 - Phase;
    // Draw dark part of moon
    display_.fillCircle(x + diameter - 1, y + diameter, diameter / 2 + 1, BLACK);
    const int number_of_lines = 90;
    for (double Ypos = 0; Ypos <= 45; Ypos++) {
        double Xpos = sqrt(45 * 45 - Ypos * Ypos);
        // Determine the edges of the lighted part of the moon
        double Rpos = 2 * Xpos;
        double Xpos1, Xpos2;
        if (Phase < 0.5) {
            Xpos1 = -Xpos;
            Xpos2 = (Rpos - 2 * Phase * Rpos - Xpos);
        }
        else {
            Xpos1 = Xpos;
            Xpos2 = (Xpos - 2 * Phase * Rpos + Rpos);
        }
        // Draw light part of moon
        double pW1x = (Xpos1 + number_of_lines) / number_of_lines * diameter + x;
        double pW1y = (number_of_lines - Ypos) / number_of_lines * diameter + y;
        double pW2x = (Xpos2 + number_of_lines) / number_of_lines * diameter + x;
        double pW2y = (number_of_lines - Ypos) / number_of_lines * diameter + y;
        double pW3x = (Xpos1 + number_of_lines) / number_of_lines * diameter + x;
        double pW3y = (Ypos + number_of_lines) / number_of_lines * diameter + y;
        double pW4x = (Xpos2 + number_of_lines) / number_of_lines * diameter + x;
        double pW4y = (Ypos + number_of_lines) / number_of_lines * diameter + y;
        display_.drawLine(pW1x, pW1y, pW2x, pW2y, GRAY2);
        display_.drawLine(pW3x, pW3y, pW4x, pW4y, GRAY2);
    }
    display_.drawCircle(x + diameter - 1, y + diameter, diameter / 2 + 1, BLACK);
}

void DrawWeather::arrow(int x, int y, int asize, float aangle, int pwidth, int plength) {
    float dx = (asize + 28) * cos((aangle - 90) * PI / 180) + x; // calculate X position
    float dy = (asize + 28) * sin((aangle - 90) * PI / 180) + y; // calculate Y position
    float x1 = 0;           float y1 = plength;
    float x2 = pwidth / 2;  float y2 = pwidth / 2;
    float x3 = -pwidth / 2; float y3 = pwidth / 2;
    float angle = aangle * PI / 180;
    float xx1 = x1 * cos(angle) - y1 * sin(angle) + dx;
    float yy1 = y1 * cos(angle) + x1 * sin(angle) + dy;
    float xx2 = x2 * cos(angle) - y2 * sin(angle) + dx;
    float yy2 = y2 * cos(angle) + x2 * sin(angle) + dy;
    float xx3 = x3 * cos(angle) - y3 * sin(angle) + dx;
    float yy3 = y3 * cos(angle) + x3 * sin(angle) + dy;
    display_.fillTriangle(xx1, yy1, xx3, yy3, xx2, yy2, BLACK);
}

// Symbols are drawn on a relative 10x10grid and 1 scale unit = 1 drawing unit

void DrawWeather::addCloud(int x, int y, int scale, int linesize) {
    //Draw cloud outer
    display_.fillCircle(x - scale * 3, y, scale, BLACK);                      // Left most circle
    display_.fillCircle(x + scale * 3, y, scale, BLACK);                      // Right most circle
    display_.fillCircle(x - scale, y - scale, scale * 1.4, BLACK);            // left middle upper circle
    display_.fillCircle(x + scale * 1.5, y - scale * 1.3, scale * 1.75, BLACK); // Right middle upper circle
    display_.fillRect(x - scale * 3 - 1, y - scale, scale * 6, scale * 2 + 1, BLACK); // Upper and lower lines
    //Clear cloud inner
    display_.fillCircle(x - scale * 3, y, scale - linesize, GRAY2);           // Clear left most circle
    display_.fillCircle(x + scale * 3, y, scale - linesize, GRAY2);           // Clear right most circle
    display_.fillCircle(x - scale, y - scale, scale * 1.4 - linesize, GRAY2); // left middle upper circle
    display_.fillCircle(x + scale * 1.5, y - scale * 1.3, scale * 1.75 - linesize, GRAY2); // Right middle upper circle
    display_.fillRect(x - scale * 3 + 2, y - scale + linesize - 1, scale * 5.9, scale * 2 - linesize * 2 + 2, GRAY2); // Upper and lower lines
}

void DrawWeather::addRaindrop(int x, int y, int scale) {
    display_.fillCircle(x, y, scale / 2, BLACK);
    display_.fillTriangle(x - scale / 2, y, x, y - scale * 1.2, x + scale / 2, y, BLACK);
    x = x + scale * 1.6; y = y + scale / 3;
    display_.fillCircle(x, y, scale / 2, BLACK);
    display_.fillTriangle(x - scale / 2, y, x, y - scale * 1.2, x + scale / 2, y, BLACK);
}

void DrawWeather::addRain(int x, int y, int scale, bool smallIcon) {
    if (smallIcon) 
        scale *= 1.34;
    for (int d = 0; d < 4; d++) {
        addRaindrop(x + scale * (7.8 - d * 1.95) - scale * 5.2, y + scale * 2.1 - scale / 6, scale / 1.6);
    }
}

void DrawWeather::addSnow(int x, int y, int scale, bool smallIcon) {
    int dxo, dyo, dxi, dyi;
    for (int flakes = 0; flakes < 5; flakes++) {
        for (int i = 0; i < 360; i = i + 45) {
            dxo = 0.5 * scale * cos((i - 90) * 3.14 / 180); dxi = dxo * 0.1;
            dyo = 0.5 * scale * sin((i - 90) * 3.14 / 180); dyi = dyo * 0.1;
            display_.drawLine(dxo + x + flakes * 1.5 * scale - scale * 3, dyo + y + scale * 2, dxi + x + 0 + flakes * 1.5 * scale - scale * 3, dyi + y + scale * 2, BLACK);
        }
    }
}

void DrawWeather::addTStorm(int x, int y, int scale) {
    y = y + scale / 2;
    for (int i = 0; i < 5; i++) {
        display_.drawLine(x - scale * 4 + scale * i * 1.5 + 0, y + scale * 1.5, x - scale * 3.5 + scale * i * 1.5 + 0, y + scale, BLACK);
        if (scale != SmallScale) {
            display_.drawLine(x - scale * 4 + scale * i * 1.5 + 1, y + scale * 1.5, x - scale * 3.5 + scale * i * 1.5 + 1, y + scale, BLACK);
            display_.drawLine(x - scale * 4 + scale * i * 1.5 + 2, y + scale * 1.5, x - scale * 3.5 + scale * i * 1.5 + 2, y + scale, BLACK);
        }
        display_.drawLine(x - scale * 4 + scale * i * 1.5, y + scale * 1.5 + 0, x - scale * 3 + scale * i * 1.5 + 0, y + scale * 1.5 + 0, BLACK);
        if (scale != SmallScale) {
            display_.drawLine(x - scale * 4 + scale * i * 1.5, y + scale * 1.5 + 1, x - scale * 3 + scale * i * 1.5 + 0, y + scale * 1.5 + 1, BLACK);
            display_.drawLine(x - scale * 4 + scale * i * 1.5, y + scale * 1.5 + 2, x - scale * 3 + scale * i * 1.5 + 0, y + scale * 1.5 + 2, BLACK);
        }
        display_.drawLine(x - scale * 3.5 + scale * i * 1.4 + 0, y + scale * 2.5, x - scale * 3 + scale * i * 1.5 + 0, y + scale * 1.5, BLACK);
        if (scale != SmallScale) {
            display_.drawLine(x - scale * 3.5 + scale * i * 1.4 + 1, y + scale * 2.5, x - scale * 3 + scale * i * 1.5 + 1, y + scale * 1.5, BLACK);
            display_.drawLine(x - scale * 3.5 + scale * i * 1.4 + 2, y + scale * 2.5, x - scale * 3 + scale * i * 1.5 + 2, y + scale * 1.5, BLACK);
        }
    }
}

void DrawWeather::addSun(int x, int y, int scale, bool smallIcon) {
    int linesize = 3;
    if (smallIcon) 
        linesize = 1;
    display_.fillRect(x - scale * 2, y, scale * 4, linesize, BLACK);
    display_.fillRect(x, y - scale * 2, linesize, scale * 4, BLACK);
    display_.drawLine(x - scale * 1.3, y - scale * 1.3, x + scale * 1.3, y + scale * 1.3, BLACK);
    display_.drawLine(x - scale * 1.3, y + scale * 1.3, x + scale * 1.3, y - scale * 1.3, BLACK);
    if (!smallIcon) {
        display_.drawLine(1 + x - scale * 1.3, y - scale * 1.3, 1 + x + scale * 1.3, y + scale * 1.3, BLACK);
        display_.drawLine(2 + x - scale * 1.3, y - scale * 1.3, 2 + x + scale * 1.3, y + scale * 1.3, BLACK);
        display_.drawLine(3 + x - scale * 1.3, y - scale * 1.3, 3 + x + scale * 1.3, y + scale * 1.3, BLACK);
        display_.drawLine(1 + x - scale * 1.3, y + scale * 1.3, 1 + x + scale * 1.3, y - scale * 1.3, BLACK);
        display_.drawLine(2 + x - scale * 1.3, y + scale * 1.3, 2 + x + scale * 1.3, y - scale * 1.3, BLACK);
        display_.drawLine(3 + x - scale * 1.3, y + scale * 1.3, 3 + x + scale * 1.3, y - scale * 1.3, BLACK);
    }
    display_.fillCircle(x, y, scale * 1.3, WHITE);
    display_.fillCircle(x, y, scale, BLACK);
    display_.fillCircle(x, y, scale - linesize, WHITE);
}

void DrawWeather::addFog(int x, int y, int scale, int linesize, bool smallIcon) {
    if (smallIcon) {
        y -= 10;
        linesize = 1;
    }
    for (int i = 0; i < 6; i++) {
        display_.fillRect(x - scale * 3, y + scale * 1.5, scale * 6, linesize, GRAY2);
        display_.fillRect(x - scale * 3, y + scale * 2.0, scale * 6, linesize, GRAY1);
        display_.fillRect(x - scale * 3, y + scale * 2.5, scale * 6, linesize, BLACK);
    }
}

void DrawWeather::displayWXicon(int x, int y, String IconName, bool smallIcon) {
    bool showMoon = IconName.endsWith("n");

    if (IconName == "01d" || IconName == "01n")       sunny(x, y, smallIcon, showMoon);
    else if (IconName == "02d" || IconName == "02n")  mostlySunny(x, y, smallIcon, showMoon);
    else if (IconName == "03d" || IconName == "03n")  cloudy(x, y, smallIcon, showMoon);
    else if (IconName == "04d" || IconName == "04n")  mostlySunny(x, y, smallIcon, showMoon);
    else if (IconName == "09d" || IconName == "09n")  chanceRain(x, y, smallIcon, showMoon);
    else if (IconName == "10d" || IconName == "10n")  rain(x, y, smallIcon, showMoon);
    else if (IconName == "11d" || IconName == "11n")  Tstorms(x, y, smallIcon, showMoon);
    else if (IconName == "13d" || IconName == "13n")  snow(x, y, smallIcon, showMoon);
    else if (IconName == "50d")                       haze(x, y, smallIcon, showMoon);
    else if (IconName == "50n")                       fog(x, y, smallIcon, showMoon);
    else                                              noData(x, y, smallIcon);
}

void DrawWeather::sunny(int x, int y, bool smallIcon, bool showMoon) {
    int scale = SmallScale, offset = 3;
    if (!smallIcon) {
        scale = LargeScale;
        y = y - 8;
        offset = 18;
    }
    else 
        y = y - 3; // Shift up small sun icon

    if (showMoon) 
        addMoon(x, y + offset, scale, smallIcon);

    scale = scale * 1.6;
    addSun(x, y, scale, smallIcon);
}

void DrawWeather::mostlySunny(int x, int y, bool smallIcon, bool showMoon) {
    int scale = SmallScale, linesize = 3, offset = 3;

    if (!smallIcon) {
        scale = LargeScale;
        offset = 10;
    }
    else 
        linesize = 1;

    if (showMoon) 
        addMoon(x, y + offset, scale, smallIcon);

    addCloud(x, y + offset, scale, linesize);
    addSun(x - scale * 1.8, y - scale * 1.8 + offset, scale, smallIcon);
}

void DrawWeather::mostlyCloudy(int x, int y, bool smallIcon, bool showMoon) {
    int scale = SmallScale, linesize = 3;

    if (!smallIcon) {
        scale = LargeScale;
        linesize = 1;
    }

    if (showMoon) 
        addMoon(x, y, scale, smallIcon);

    addCloud(x, y, scale, linesize);
    addSun(x - scale * 1.8, y - scale * 1.8, scale, smallIcon);
    addCloud(x, y, scale, linesize);
}

void DrawWeather::cloudy(int x, int y, bool smallIcon, bool showMoon) {
    int scale = LargeScale, linesize = 3;

    if (smallIcon) {
        scale = SmallScale;
        if (showMoon) 
            addMoon(x, y, scale, smallIcon);
        linesize = 1;
        addCloud(x, y, scale, linesize);
    }
    else {
        y += 10;
        if (showMoon) 
            addMoon(x, y, scale, smallIcon);
        addCloud(x + 30, y - 35, 5, linesize); // Cloud top right
        addCloud(x - 20, y - 25, 7, linesize); // Cloud top left
        addCloud(x, y, scale, linesize);       // Main cloud
    }
}

void DrawWeather::rain(int x, int y, bool smallIcon, bool showMoon) {
    int scale = LargeScale, linesize = 3;

    if (smallIcon) {
        scale = SmallScale;
        linesize = 1;
    }

    if (showMoon) 
        addMoon(x, y + 10, scale, smallIcon);

    addCloud(x, y, scale, linesize);
    addRain(x, y, scale, smallIcon);
}

void DrawWeather::expectRain(int x, int y, bool smallIcon, bool showMoon) {
    int scale = LargeScale, linesize = 3;

    if (smallIcon) {
        scale = SmallScale;
        linesize = 1;
    }

    if (showMoon) 
        addMoon(x, y, scale, smallIcon);

    addSun(x - scale * 1.8, y - scale * 1.8, scale, smallIcon);
    addCloud(x, y, scale, linesize);
    addRain(x, y, scale, smallIcon);
}

void DrawWeather::chanceRain(int x, int y, bool smallIcon, bool showMoon) {
    int scale = LargeScale, linesize = 3;

    if (smallIcon) {
        scale = SmallScale;
        linesize = 1;
    }

    if (showMoon) 
        addMoon(x, y, scale, smallIcon);

    addSun(x - scale * 1.8, y - scale * 1.8, scale, smallIcon);
    addCloud(x, y, scale, linesize);
    addRain(x, y, scale, smallIcon);
}

void DrawWeather::Tstorms(int x, int y, bool smallIcon, bool showMoon) {
    int scale = LargeScale, linesize = 3;

    if (smallIcon) {
        scale = SmallScale;
        linesize = 1;
    }

    if (showMoon) 
        addMoon(x, y, scale, smallIcon);

    addCloud(x, y, scale, linesize);
    addTStorm(x, y, scale);
}

void DrawWeather::snow(int x, int y, bool smallIcon, bool showMoon) {
    int scale = LargeScale, linesize = 3;

    if (smallIcon) {
        scale = SmallScale;
        linesize = 1;
    }

    if (showMoon) 
        addMoon(x, y + 15, scale, smallIcon);

    addCloud(x, y, scale, linesize);
    addSnow(x, y, scale, smallIcon);
}

void DrawWeather::fog(int x, int y, bool smallIcon, bool showMoon) {
    int linesize = 3, scale = LargeScale;

    if (smallIcon) {
        scale = SmallScale;
        linesize = 1;
    }

    if (showMoon) 
        addMoon(x, y, scale, smallIcon);

    addCloud(x, y - 5, scale, linesize);
    addFog(x, y - 5, scale, linesize, smallIcon);
}

void DrawWeather::haze(int x, int y, bool smallIcon, bool showMoon) {
    int linesize = 3, scale = LargeScale;

    if (smallIcon) {
        scale = SmallScale;
        linesize = 1;
    }

    if (showMoon) 
        addMoon(x, y, scale, smallIcon);

    addSun(x, y - 5, scale * 1.4, smallIcon);
    addFog(x, y - 5, scale * 1.4, linesize, smallIcon);
}

void DrawWeather::cloudCover(int x, int y, int CCover) {
    constexpr int dx = 45;
    constexpr int dy = 10;
    constexpr int scale = SmallScale * 0.5;
    addCloud(x - 9 - dx, y - 3 + dy, scale, 2); // Cloud top left
    addCloud(x + 3 - dx, y - 3 + dy, scale, 2); // Cloud top right
    addCloud(x - dx, y + dy, scale, 2); // Main cloud
    //u8g2Fonts.setFont(u8g2_font_helvB08_tf);
    display_.drawString(x, y, String(CCover) + "%", RIGHT);
}

void DrawWeather::visibility(int x, int y, String Visi) {
    y = y - 3; //
    float start_angle = 0.52, end_angle = 2.61;
    int r = 10;
    for (float i = start_angle; i < end_angle; i = i + 0.05) {
        display_.setPoint(x + r * cos(i), y - r / 2 + r * sin(i), BLACK);
        display_.setPoint(x + r * cos(i), 1 + y - r / 2 + r * sin(i), BLACK);
    }
    start_angle = 3.61; end_angle = 5.78;
    for (float i = start_angle; i < end_angle; i = i + 0.05) {
        display_.setPoint(x + r * cos(i), y + r / 2 + r * sin(i), BLACK);
        display_.setPoint(x + r * cos(i), 1 + y + r / 2 + r * sin(i), BLACK);
    }
    display_.fillCircle(x, y, r / 4, BLACK);
    //u8g2Fonts.setFont(u8g2_font_helvB08_tf);
    display_.drawString(x + 12, y - 3, Visi, LEFT);
}

void DrawWeather::addMoon(int x, int y, int scale, bool smallIcon) {
    if (!smallIcon) {
        x = x + 12; y = y + 12;
        display_.fillCircle(x - 50, y - 55, scale, GRAY1);
        display_.fillCircle(x - 35, y - 55, scale * 1.6, WHITE);
    }
    else
    {
        display_.fillCircle(x - 20, y - 12, scale, GRAY1);
        display_.fillCircle(x - 15, y - 12, scale * 1.6, WHITE);
    }
}

void DrawWeather::noData(int x, int y, bool smallIcon) {
    //if (!smallIcon) 
    //    u8g2Fonts.setFont(u8g2_font_helvB24_tf); 
    //else 
    //    u8g2Fonts.setFont(u8g2_font_helvB10_tf);

    display_.drawString(x - 3, y - 8, "?", CENTER);
    //u8g2Fonts.setFont(u8g2_font_helvB08_tf);
}

void DrawWeather::drawGraph(int x_pos, int y_pos, int gwidth, int gheight, float Y1Min, float Y1Max, String title, float DataArray[], int readings, boolean auto_scale, boolean barchart_mode) {
#define auto_scale_margin 0 // Sets the autoscale increment, so axis steps up in units of e.g. 3
#define y_minor_axis 5      // 5 y-axis division markers
    float maxYscale = -10000;
    float minYscale = 10000;
    int last_x, last_y;
    float x1, y1, x2, y2;
    if (auto_scale == true) {
        for (int i = 1; i < readings; i++) {
            if (DataArray[i] >= maxYscale) maxYscale = DataArray[i];
            if (DataArray[i] <= minYscale) minYscale = DataArray[i];
        }
        maxYscale = round(maxYscale + auto_scale_margin); // Auto scale the graph and round to the nearest value defined, default was Y1Max
        Y1Max = round(maxYscale + 0.5);
        if (minYscale != 0) minYscale = round(minYscale - auto_scale_margin); // Auto scale the graph and round to the nearest value defined, default was Y1Min
        Y1Min = round(minYscale);
    }
    // Draw the graph
    last_x = x_pos + 1;
    last_y = y_pos + (Y1Max - constrain(DataArray[1], Y1Min, Y1Max)) / (Y1Max - Y1Min) * gheight;
    display_.drawRect(x_pos, y_pos, gwidth + 3, gheight + 2, GRAY1);
    //u8g2Fonts.setFont(u8g2_font_helvB08_tf);
    display_.drawString(x_pos + gwidth / 2, y_pos - 12, title, CENTER);
    // Draw the data
    for (int gx = 1; gx < readings; gx++) {
        x1 = last_x;
        y1 = last_y;
        x2 = x_pos + gx * gwidth / (readings - 1) - 1; // max_readings is the global variable that sets the maximum data that can be plotted
        y2 = y_pos + (Y1Max - constrain(DataArray[gx], Y1Min, Y1Max)) / (Y1Max - Y1Min) * gheight + 1;
        if (barchart_mode) {
            display_.fillRect(x2, y2, (gwidth / readings) - 1, y_pos + gheight - y2 + 1, BLACK);
        }
        else {
            display_.drawLine(last_x, last_y, x2, y2, BLACK);
        }
        last_x = x2;
        last_y = y2;
    }
    //Draw the Y-axis scale
#define number_of_dashes 15
    for (int spacing = 0; spacing <= y_minor_axis; spacing++) {
        for (int j = 0; j < number_of_dashes; j++) { // Draw dashed graph grid lines
            if (spacing < y_minor_axis) 
                display_.drawHLine((x_pos + 3 + j * gwidth / number_of_dashes), y_pos + (gheight * spacing / y_minor_axis), gwidth / (2 * number_of_dashes), GRAY1);
        }
        if ((Y1Max - (float)(Y1Max - Y1Min) / y_minor_axis * spacing) < 5) {
            display_.drawString(x_pos, y_pos + gheight * spacing / y_minor_axis - 5, String((Y1Max - (float)(Y1Max - Y1Min) / y_minor_axis * spacing + 0.01), 1), RIGHT);
        }
        else
        {
            if (Y1Min < 1 && Y1Max < 10)
                display_.drawString(x_pos - 3, y_pos + gheight * spacing / y_minor_axis - 5, String((Y1Max - (float)(Y1Max - Y1Min) / y_minor_axis * spacing + 0.01), 1), RIGHT);
            else
                display_.drawString(x_pos - 3, y_pos + gheight * spacing / y_minor_axis - 5, String((Y1Max - (float)(Y1Max - Y1Min) / y_minor_axis * spacing + 0.01), 0), RIGHT);
        }
    }
    for (int i = 0; i <= 2; i++) {
        display_.drawString(15 + x_pos + gwidth / 3 * i, y_pos + gheight + 3, String(i), LEFT);
    }
    display_.drawString(x_pos + gwidth / 2, y_pos + gheight + 10, TXT_DAYS, CENTER);
}


void DrawWeather::drawHeadingSection(String City) {
    //display_.drawHLine(0, 17, Gray2bitMap::WIDTH, GRAY2);

    struct tm timeinfo;
    getLocalTime(&timeinfo);
    char time_output[30], day_output[30];
    snprintf(day_output, 30, "%i년 %i월 %i일 %s", (timeinfo.tm_year) + 1900, timeinfo.tm_mon + 1, timeinfo.tm_mday, weekday_D[timeinfo.tm_wday]);

    if(timeinfo.tm_hour < 12) {
      snprintf(time_output, 30, "오전 %02i시 %02i분", timeinfo.tm_hour == 0 ? 12 : timeinfo.tm_hour, timeinfo.tm_min);
    } else {
      snprintf(time_output, 30, "오후 %02i시 %02i분", timeinfo.tm_hour == 12 ? 12 : timeinfo.tm_hour - 12, timeinfo.tm_min);
    }
  
    int ix = 4;
    int iy = 2;
    display_.drawString(ix, iy, City, LEFT);
    iy += 17;
    display_.drawString(ix, iy, day_output, LEFT);
    iy += 16;
    display_.drawString(ix, iy, time_output, LEFT);
   
    
}

String windDegToDirection(float winddirection) {
  if (winddirection >= 348.75 || winddirection < 11.25)  return TXT_N;
  if (winddirection >=  11.25 && winddirection < 33.75)  return TXT_NNE;
  if (winddirection >=  33.75 && winddirection < 56.25)  return TXT_NE;
  if (winddirection >=  56.25 && winddirection < 78.75)  return TXT_ENE;
  if (winddirection >=  78.75 && winddirection < 101.25) return TXT_E;
  if (winddirection >= 101.25 && winddirection < 123.75) return TXT_ESE;
  if (winddirection >= 123.75 && winddirection < 146.25) return TXT_SE;
  if (winddirection >= 146.25 && winddirection < 168.75) return TXT_SSE;
  if (winddirection >= 168.75 && winddirection < 191.25) return TXT_S;
  if (winddirection >= 191.25 && winddirection < 213.75) return TXT_SSW;
  if (winddirection >= 213.75 && winddirection < 236.25) return TXT_SW;
  if (winddirection >= 236.25 && winddirection < 258.75) return TXT_WSW;
  if (winddirection >= 258.75 && winddirection < 281.25) return TXT_W;
  if (winddirection >= 281.25 && winddirection < 303.75) return TXT_WNW;
  if (winddirection >= 303.75 && winddirection < 326.25) return TXT_NW;
  if (winddirection >= 326.25 && winddirection < 348.75) return TXT_NNW;
  return "?";
}

void DrawWeather::displayDisplayWindSection(int x, int y, float angle, float windspeed, int Cradius) {
  arrow(x, y, Cradius - 7, angle, 12, 18); // Show wind direction on outer circle of width and length

  int dxo, dyo, dxi, dyi;
  display_.drawLine(0, 15, 0, y + Cradius + 30, BLACK);
  display_.drawCircle(x, y, Cradius, BLACK);     // Draw compass circle
  display_.drawCircle(x, y, Cradius + 1, BLACK); // Draw compass circle
  display_.drawCircle(x, y, Cradius * 0.7, BLACK); // Draw compass inner circle
  for (float a = 0; a < 360; a = a + 22.5) {
    dxo = Cradius * cos((a - 90) * PI / 180);
    dyo = Cradius * sin((a - 90) * PI / 180);

    if (a == 45)  
        display_.drawString(dxo + x + 10, dyo + y - 10, TXT_NE, CENTER);
    if (a == 135) 
        display_.drawString(dxo + x + 7,  dyo + y + 5,  TXT_SE, CENTER);
    if (a == 225) 
        display_.drawString(dxo + x - 15, dyo + y,      TXT_SW, CENTER);
    if (a == 315) 
        display_.drawString(dxo + x - 15, dyo + y - 10, TXT_NW, CENTER);

    dxi = dxo * 0.9;
    dyi = dyo * 0.9;
    display_.drawLine(dxo + x, dyo + y, dxi + x, dyi + y, BLACK);
    dxo = dxo * 0.7;
    dyo = dyo * 0.7;
    dxi = dxo * 0.9;
    dyi = dyo * 0.9;
    display_.drawLine(dxo + x, dyo + y, dxi + x, dyi + y, BLACK);
  }
  display_.drawString(x, y - Cradius - 10,     TXT_N, CENTER);
  display_.drawString(x, y + Cradius + 5,      TXT_S, CENTER);
  display_.drawString(x - Cradius - 10, y - 3, TXT_W, CENTER);
  display_.drawString(x + Cradius + 8,  y - 3, TXT_E, CENTER);
  display_.drawString(x - 2, y - 20, windDegToDirection(angle), CENTER);
  display_.drawString(x + 3, y + 12, String(angle, 0) + "\'", CENTER);
  display_.drawString(x + 3, y - 3, String(windspeed, 1) + "m/s", CENTER);
}

void DrawWeather::drawPressureAndTrend(int x, int y, float pressure, String slope) {
  display_.drawString(x, y, String(pressure, 0) + "mb", CENTER);
  x = x + 40; y = y + 2;
  if      (slope == "+") {
    display_.drawLine(x,  y, x + 4, y - 4, BLACK);
    display_.drawLine(x + 4, y - 4, x + 8, y, BLACK);
  }
  else if (slope == "0") {
    display_.drawLine(x + 4, y - 4, x + 8, y, BLACK);
    display_.drawLine(x + 4, y + 4, x + 8, y, BLACK);
  }
  else if (slope == "-") {
    display_.drawLine(x,  y, x + 4, y + 4, BLACK);
    display_.drawLine(x + 4, y + 4, x + 8, y, BLACK);
  }
}


void DrawWeather::drawStringMaxWidth(int x, int y, unsigned int text_width, String text, Alignment align) {
  uint16_t w, h;
  display_.getTextBounds(text, &w, &h);
  if (align == RIGHT)  
    x = x - w;
  if (align == CENTER) 
    x = x - w / 2;
  
  if (text.length() > text_width * 2) {    
    text_width = 42;
    y = y - 3;
  }

  display_.drawString(x, y, text.substring(0, text_width));
  if (text.length() > text_width) {
    String secondLine = text.substring(text_width);
    secondLine.trim(); // Remove any leading spaces
    display_.drawString(x, y + h + 17, secondLine);
  }
}

void DrawWeather::drawMainWx(int x, int y, Forecast_record_type& weather) {
    display_.drawString(x, y, String(weather.Temperature, 1) + STR_CELSIUS, LEFT); // Show current Temperature 
    y += 18;
    display_.drawString(x, y, String("습도 ") + String(weather.Humidity, 0) + "%", LEFT);
    y += 18;
    display_.drawString(x, y, String("풍속 ") + String(weather.Windspeed, 1) + "m/s", LEFT);

    y += 18;
    if(MAX(weather.Rainfall, weather.Snowfall) > 0.05) {
        if(weather.Rainfall > weather.Snowfall) {
            addRaindrop(x, y+8, 7);
            display_.drawString(x + 20, y, String(weather.Rainfall, 1) + "mm", LEFT); // Only display rainfall total today if > 0
        } else 
            display_.drawString(x - 5, y, String("* * ") + String(weather.Snowfall, 1) + "mm", LEFT); // Only display snowfall total today if > 0
    }
}

void DrawWeather::displayPrecipitationSection(int x, int y, Forecast_record_type& forecast) {
  //display_.drawRect(x, y - 1, 167, 56, GRAY1); // precipitation outline

  if (forecast.Rainfall > 0.005) { // Ignore small amounts
    addRaindrop(x, y, 7);
    display_.drawString(x + 24, y, String(forecast.Rainfall, 2) + "mm", LEFT); // Only display rainfall total today if > 0
  }
  if (forecast.Snowfall > 0.005)  // Ignore small amounts
    display_.drawString(x - 4, y, String("* * ") + String(forecast.Snowfall, 2) + "mm", LEFT); // Only display snowfall total today if > 0
}

void DrawWeather::drawMainWeatherSection(int x, int y, Forecast_record_type& weather) {
    //displayDisplayWindSection(x - 115, y - 3, weather.Winddir, weather.Windspeed, 40);
    displayWXicon(x - 30, y, weather.Icon, false);
    //drawPressureAndTrend(x - 120, y + 58, weather.Pressure, weather.Trend);
  
    String Wx_Description = weather.Forecast0;
    if (weather.Forecast1 != "") 
      Wx_Description += " & " +  weather.Forecast1;
    if (weather.Forecast2 != "" && weather.Forecast1 != weather.Forecast2) 
      Wx_Description += " & " +  weather.Forecast2;
    drawStringMaxWidth(x - 30, y + 44, 20, titleCase(Wx_Description), CENTER);
    //display_.drawRect(0, y + 68, 232, 48, GRAY1);

    x += 45;
    y -= 40;
    drawMainWx(x, y, weather);
    //x += 50;
    //y += 36;
    //displayPrecipitationSection(x, y, weather);
}

void DrawWeather::drawForecastWeather(int x, int y, Forecast_record_type &forecast) {
  display_.drawRect(x, y, 55, 65, GRAY1);
  display_.drawLine(x + 1, y + 16, x + 54, y + 16, GRAY1);
  displayWXicon(x + 28, y + 35, forecast.Icon, true);
  display_.drawString(x + 31, y + 3, String(convertUnixTime(forecast.Dt).substring(0,5)), CENTER);
  display_.drawString(x + 41, y + 52, String(forecast.High, 0) + STR_DOT + "/" + String(forecast.Low, 0) + STR_DOT, CENTER);
}

void DrawWeather::drawForecastSection(int x, int y, Forecast_record_type* forecast) {
    display_.drawHLine(0, y - 1, Gray2bitMap::WIDTH, GRAY2);

    constexpr int step = 55;
    constexpr int iconCenter = 33;

    float temperature_readings[OpenWeather::maxReadings_] = {0};
    float rain_readings[OpenWeather::maxReadings_] = {0};

    bool isRain = false;
    float tmax = -999, tmin = 999;
    for (int i = 0; i < OpenWeather::maxReadings_; ++i) {
        displayWXicon(iconCenter + i*step, y + 36, forecast[i].Icon, true);
        display_.drawString(iconCenter + i*step, y, String(convertUnixTime(forecast[i].Dt).substring(0,5)), CENTER, GRAY1);

        temperature_readings[i] = forecast[i].Temperature;
        tmax = MAX(tmax, temperature_readings[i]);
        tmin = MIN(tmin, temperature_readings[i]);

        rain_readings[i] = MAX(forecast[i].Rainfall, forecast[i].Snowfall);
        if(rain_readings[i] > 0.005)
            isRain = true;
    }

    y += 53;
    if(isRain) {
        for (int i = 0; i < OpenWeather::maxReadings_; ++i) {
            if(rain_readings[i] > 0.05)
                display_.drawString(iconCenter + i*step, y, String(rain_readings[i], 1) + String("mm"), CENTER);
        }

        y += 16;
    }

    y += 4;
    float scale = 42 / (tmax - tmin);

    for (int i = 0; i < OpenWeather::maxReadings_; ++i) {        
        int x0 = iconCenter + i*step;
        int y0 = y + scale*(tmax - temperature_readings[i]);
        display_.fillRect(x0-1, y0-1, 3, 3, BLACK);
        //display_.fillCircle(x0, y0, 2, BLACK);
    }

    int x0 = iconCenter;
    int y0 = y + scale*(tmax - temperature_readings[0]);

    for (int i = 1; i < OpenWeather::maxReadings_; ++i) 
    {
        int x1 = iconCenter + i*step;
        int y1 = y + scale*(tmax - temperature_readings[i]);
        display_.drawLine(x0, y0, x1, y1, GRAY1);
        x0 = x1;
        y0 = y1;
    }

    const float cc = (tmax + tmin) / 2.f;
    for (int i = 0; i < OpenWeather::maxReadings_; ++i) {
        float dist = tmax - temperature_readings[i];

        if(temperature_readings[i] < cc) {
            display_.drawString(iconCenter + i*step, y + scale*dist - 21, String(temperature_readings[i], 1) + STR_DOT, CENTER);
        } else {
            display_.drawString(iconCenter + i*step, y + scale*dist + 5, String(temperature_readings[i], 1) + STR_DOT, CENTER);
        }
    }
}

void DrawWeather::drawAstronomySection(Forecast_record_type& weather) 
{
    int x = Gray2bitMap::WIDTH - 4;
    int y = 2 + 17;
    //display_.drawRect(x, y + 64, 167, 48, GRAY1);

    display_.drawString(x, y, String(TXT_SUNRISE) + " " + convertUnixTime(weather.Sunrise).substring(0, 5), RIGHT);
    y += 16;
    display_.drawString(x, y, String(TXT_SUNSET) + " " + convertUnixTime(weather.Sunset).substring(0, 5), RIGHT);
    y += 10;

    time_t now = time(NULL);
    struct tm * now_utc = gmtime(&now);
    const int day_utc   = now_utc->tm_mday;
    const int month_utc = now_utc->tm_mon + 1;
    const int year_utc  = now_utc->tm_year + 1900;
    drawMoon(x - 65, y, day_utc, month_utc, year_utc);
    y += 63;
    display_.drawString(x, y, moonPhase(day_utc, month_utc, year_utc), RIGHT);
    
    //if (weather.Visibility > 0) 
    //  visibility(335, 100, String(weather.Visibility) + "m");
    y += 22;
    if (weather.Cloudcover > 0) 
      cloudCover(x, y, weather.Cloudcover);
}


void DrawWeather::displayWeather(OpenWeather &data) {
    drawHeadingSection(data.City_);                 // Top line of the display
    drawAstronomySection(data.weatherData_);        // Astronomy section Sun rise/set, Moon phase and Moon icon    
    drawMainWeatherSection(140, 110, data.weatherData_);      // Centre section of display for Location, temperature, Weather report, current Wx Symbol and wind direction
    drawForecastSection(10, 180, data.forecastData_);         // 3hr forecast boxes
}