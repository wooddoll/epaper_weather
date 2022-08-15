#ifndef _GRAY_2BIT_MAP_H_
#define _GRAY_2BIT_MAP_H_

#include <Arduino.h>
#include <stdint.h>

enum COLOR_4GRAY { BLACK=0, GRAY1=1, GRAY2=2, WHITE=3 };
enum Alignment { LEFT, CENTER, RIGHT };

class Gray2bitMap {
public:
    Gray2bitMap() {
        buffer_ = new uint8_t [bufferByte_];
    }
    ~Gray2bitMap() {
        if(buffer_)
            delete[] buffer_;
    }

    // color must be b00, b01, b10, b11
    void setTo(uint8_t color);
    void setTo(uint8_t* ptr);
    void setPoint(uint16_t x, uint16_t y, uint8_t color);
    void operator() (uint16_t x, uint16_t y, uint8_t color) {
        setPoint(x, y, color);
    }
    void drawLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint8_t color);

    void drawCircle(int16_t x0, int16_t y0, int16_t r, uint8_t color);
    void drawRect(int16_t x, int16_t y, int16_t w, int16_t h, uint8_t color);

    void fillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint8_t color);
    void fillCircle(int16_t x0, int16_t y0, int16_t r, uint8_t color);

    void drawHLine(int16_t x, int16_t y, int16_t w, uint8_t color) {
        drawLine(x, y, x + w - 1, y, color);
    }
    void drawVLine(int16_t x, int16_t y, int16_t h, uint8_t color) {
        drawLine(x, y, x, y + h - 1, color);
    }

    void drawRoundRect(int16_t x, int16_t y, int16_t w, int16_t h, int16_t r, uint8_t color);
    void fillRoundRect(int16_t x, int16_t y, int16_t w, int16_t h, int16_t r, uint8_t color);

    void drawTriangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint8_t color);
    void fillTriangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint8_t color);

    void drawMonoMapToColor(uint16_t x, uint16_t y, uint8_t *image, uint16_t width, uint16_t height, uint8_t color);
    void putString(uint16_t x, uint16_t y, const String &text, uint8_t color = 0);

    void getTextBounds(const String &str, uint16_t *w, uint16_t *h);
    void drawString(int x, int y, const String &text, Alignment align = Alignment::LEFT, uint8_t color = 0);

private:
    void drawCircleHelper(int16_t x0, int16_t y0, int16_t r, uint8_t cornername, uint8_t color);
    void fillCircleHelper(int16_t x0, int16_t y0, int16_t r, uint8_t corners, int16_t delta, uint8_t color);

public:
    static constexpr uint16_t WIDTH = 400;
    static constexpr uint16_t HEIGHT = 300;
    static constexpr uint32_t widthByte_ = WIDTH / 4;
    static constexpr uint32_t bufferByte_ = widthByte_ * HEIGHT;
    //static uint8_t buffer_[bufferByte_];
    static uint8_t* buffer_;
};

#endif//_GRAY_2BIT_MAP_H_