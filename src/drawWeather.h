#ifndef _DRAW_WEATHER_H_
#define _DRAW_WEATHER_H_

#include "gray2bitmap.h"
#include "weathertime.h"


enum Hemisphere { NORTH, SOUTH };

class DrawWeather {
public:
    DrawWeather(Gray2bitMap &bitmap) : display_(bitmap) {}
    ~DrawWeather() {}

    String moonPhase(int d, int m, int y);
    void drawMoon(int x, int y, int dd, int mm, int yy);
    void arrow(int x, int y, int asize, float aangle, int pwidth, int plength);
    void addCloud(int x, int y, int scale, int linesize);
    void addRaindrop(int x, int y, int scale);
    void addRain(int x, int y, int scale, bool IconSize);
    void addSnow(int x, int y, int scale, bool IconSize);
    void addTStorm(int x, int y, int scale);
    void addSun(int x, int y, int scale, bool IconSize);
    void addFog(int x, int y, int scale, int linesize, bool IconSize);
    void addMoon(int x, int y, int scale, bool IconSize);

    void displayWXicon(int x, int y, String IconName, bool IconSize);
    
    void sunny(int x, int y, bool IconSize, bool showMoon);
    void mostlySunny(int x, int y, bool IconSize, bool showMoon);
    void mostlyCloudy(int x, int y, bool IconSize, bool showMoon);
    void cloudy(int x, int y, bool IconSize, bool showMoon);
    void rain(int x, int y, bool IconSize, bool showMoon);
    void expectRain(int x, int y, bool IconSize, bool showMoon);
    void chanceRain(int x, int y, bool IconSize, bool showMoon);
    void Tstorms(int x, int y, bool IconSize, bool showMoon);
    void snow(int x, int y, bool IconSize, bool showMoon);
    void fog(int x, int y, bool IconSize, bool showMoon);
    void haze(int x, int y, bool IconSize, bool showMoon);    
    void noData(int x, int y, bool IconSize);

    void cloudCover(int x, int y, int CCover);
    void visibility(int x, int y, String Visi);    
    void drawGraph(int x_pos, int y_pos, int gwidth, int gheight, float Y1Min, float Y1Max, 
            String title, float DataArray[], int readings, 
            boolean auto_scale, boolean barchart_mode);

    void displayDisplayWindSection(int x, int y, float angle, float windspeed, int Cradius);
    void drawPressureAndTrend(int x, int y, float pressure, String slope);

    void drawHeadingSection(String City);
    void drawMainWeatherSection(int x, int y, Forecast_record_type& weather);
    void drawForecastSection(int x, int y, Forecast_record_type* forecast);
    void displayPrecipitationSection(int x, int y, Forecast_record_type& forecast);
    void drawAstronomySection(Forecast_record_type& weather);

    void drawStringMaxWidth(int x, int y, unsigned int text_width, String text, Alignment align);
    void drawMainWx(int x, int y, Forecast_record_type& weather);
    void drawForecastWeather(int x, int y, Forecast_record_type &forecast);

    void displayWeather(OpenWeather &data);
public:
    static constexpr int LargeScale = 13;
    static constexpr int SmallScale = 5;
    static constexpr Hemisphere hemisphere_ = Hemisphere::NORTH;
    Gray2bitMap &display_;
};

#endif//_DRAW_WEATHER_H_