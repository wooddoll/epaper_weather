#include "gray2bitmap.h"

#include <string.h>
#include <stdlib.h>

//uint8_t Gray2bitMap::buffer_[30000] = { 0, };
uint8_t* Gray2bitMap::buffer_ = NULL;

template<typename T>
inline void swap(T &a, T &b) {
    T tmp = a;
    a = b;
    b = tmp;
}

void Gray2bitMap::setTo(uint8_t color) {
    color &= 3;
    color = (color << 6) | (color << 4) | (color << 2) | color;

    memset(buffer_, color, bufferByte_);
}

void Gray2bitMap::setTo(uint8_t* ptr) {
    memcpy(buffer_, ptr, bufferByte_);
}

void Gray2bitMap::setPoint(uint16_t x, uint16_t y, uint8_t color) {
    color &= 3; //Guaranteed color scale is 4  --- 0~3

    uint8_t &Rdata = buffer_[(x >> 2) + y * widthByte_];

    x = (3 - (x & 3)) << 1;
    Rdata &= ~(3 << x);     //Clear first, then set value
    Rdata |= color << x;
}

void Gray2bitMap::drawLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint8_t color) {
    int16_t steep = abs(y1 - y0) > abs(x1 - x0);

    if (steep) {
        swap(x0, y0);
        swap(x1, y1);
    }

    if (x0 > x1) {
        swap(x0, x1);
        swap(y0, y1);
    }

    int16_t dx, dy;
    dx = x1 - x0;
    dy = abs(y1 - y0);

    int16_t err = dx / 2;
    int16_t ystep;

    if (y0 < y1) {
        ystep = 1;
    }
    else {
        ystep = -1;
    }

    for (; x0 <= x1; x0++) {
        if (steep) {
            setPoint(y0, x0, color);
        }
        else {
            setPoint(x0, y0, color);
        }
        err -= dy;
        if (err < 0) {
            y0 += ystep;
            err += dx;
        }
    }
}


void Gray2bitMap::drawCircle(int16_t x0, int16_t y0, int16_t r, uint8_t color) {
    int16_t f = 1 - r;
    int16_t ddF_x = 1;
    int16_t ddF_y = -2 * r;
    int16_t x = 0;
    int16_t y = r;

    setPoint(x0, y0 + r, color);
    setPoint(x0, y0 - r, color);
    setPoint(x0 + r, y0, color);
    setPoint(x0 - r, y0, color);

    while (x < y) {
        if (f >= 0) {
            y--;
            ddF_y += 2;
            f += ddF_y;
        }
        x++;
        ddF_x += 2;
        f += ddF_x;

        setPoint(x0 + x, y0 + y, color);
        setPoint(x0 - x, y0 + y, color);
        setPoint(x0 + x, y0 - y, color);
        setPoint(x0 - x, y0 - y, color);
        setPoint(x0 + y, y0 + x, color);
        setPoint(x0 - y, y0 + x, color);
        setPoint(x0 + y, y0 - x, color);
        setPoint(x0 - y, y0 - x, color);
    }
}

void Gray2bitMap::drawRect(int16_t x, int16_t y, int16_t w, int16_t h, uint8_t color) {
    drawHLine(x, y, w, color);
    drawHLine(x, y + h - 1, w, color);
    drawVLine(x, y, h, color);
    drawVLine(x + w - 1, y, h, color);
}

void Gray2bitMap::fillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint8_t color) {
    for (int16_t i = x; i < x + w; i++) {
        drawVLine(i, y, h, color);
    }
}

void Gray2bitMap::drawCircleHelper(int16_t x0, int16_t y0, int16_t r, uint8_t cornername, uint8_t color)
{
    int16_t f = 1 - r;
    int16_t ddF_x = 1;
    int16_t ddF_y = -2 * r;
    int16_t x = 0;
    int16_t y = r;

    while (x < y) {
        if (f >= 0) {
            y--;
            ddF_y += 2;
            f += ddF_y;
        }
        x++;
        ddF_x += 2;
        f += ddF_x;
        if (cornername & 0x4) {
            setPoint(x0 + x, y0 + y, color);
            setPoint(x0 + y, y0 + x, color);
        }
        if (cornername & 0x2) {
            setPoint(x0 + x, y0 - y, color);
            setPoint(x0 + y, y0 - x, color);
        }
        if (cornername & 0x8) {
            setPoint(x0 - y, y0 + x, color);
            setPoint(x0 - x, y0 + y, color);
        }
        if (cornername & 0x1) {
            setPoint(x0 - y, y0 - x, color);
            setPoint(x0 - x, y0 - y, color);
        }
    }
}

void Gray2bitMap::fillCircleHelper(int16_t x0, int16_t y0, int16_t r, uint8_t corners, int16_t delta, uint8_t color)
{
    int16_t f = 1 - r;
    int16_t ddF_x = 1;
    int16_t ddF_y = -2 * r;
    int16_t x = 0;
    int16_t y = r;
    int16_t px = x;
    int16_t py = y;

    delta++; // Avoid some +1's in the loop

    while (x < y) {
        if (f >= 0) {
            y--;
            ddF_y += 2;
            f += ddF_y;
        }
        x++;
        ddF_x += 2;
        f += ddF_x;
        // These checks avoid double-drawing certain lines, important
        // for the SSD1306 library which has an INVERT drawing mode.
        if (x < (y + 1)) {
            if (corners & 1)
                drawVLine(x0 + x, y0 - y, 2 * y + delta, color);
            if (corners & 2)
                drawVLine(x0 - x, y0 - y, 2 * y + delta, color);
        }
        if (y != py) {
            if (corners & 1)
                drawVLine(x0 + py, y0 - px, 2 * px + delta, color);
            if (corners & 2)
                drawVLine(x0 - py, y0 - px, 2 * px + delta, color);
            py = y;
        }
        px = x;
    }
}

void Gray2bitMap::fillCircle(int16_t x0, int16_t y0, int16_t r, uint8_t color) {
    drawVLine(x0, y0 - r, 2 * r + 1, color);
    fillCircleHelper(x0, y0, r, 3, 0, color);
}

void Gray2bitMap::drawRoundRect(int16_t x, int16_t y, int16_t w, int16_t h, int16_t r, uint8_t color)
{
    int16_t max_radius = ((w < h) ? w : h) / 2; // 1/2 minor axis
    if (r > max_radius)
        r = max_radius;
    // smarter version
    drawHLine(x + r, y, w - 2 * r, color);         // Top
    drawHLine(x + r, y + h - 1, w - 2 * r, color); // Bottom
    drawVLine(x, y + r, h - 2 * r, color);         // Left
    drawVLine(x + w - 1, y + r, h - 2 * r, color); // Right
    // draw four corners
    drawCircleHelper(x + r, y + r, r, 1, color);
    drawCircleHelper(x + w - r - 1, y + r, r, 2, color);
    drawCircleHelper(x + w - r - 1, y + h - r - 1, r, 4, color);
    drawCircleHelper(x + r, y + h - r - 1, r, 8, color);
}

void Gray2bitMap::fillRoundRect(int16_t x, int16_t y, int16_t w, int16_t h, int16_t r, uint8_t color)
{
    int16_t max_radius = ((w < h) ? w : h) / 2; // 1/2 minor axis
    if (r > max_radius)
        r = max_radius;
    // smarter version
    fillRect(x + r, y, w - 2 * r, h, color);
    // draw four corners
    fillCircleHelper(x + w - r - 1, y + r, r, 1, h - 2 * r - 1, color);
    fillCircleHelper(x + r, y + r, r, 2, h - 2 * r - 1, color);
}

void Gray2bitMap::drawTriangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint8_t color)
{
    drawLine(x0, y0, x1, y1, color);
    drawLine(x1, y1, x2, y2, color);
    drawLine(x2, y2, x0, y0, color);
}

void Gray2bitMap::fillTriangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint8_t color)
{
    int16_t a, b, y, last;

    // Sort coordinates by Y order (y2 >= y1 >= y0)
    if (y0 > y1) {
        swap(y0, y1);
        swap(x0, x1);
    }
    if (y1 > y2) {
        swap(y2, y1);
        swap(x2, x1);
    }
    if (y0 > y1) {
        swap(y0, y1);
        swap(x0, x1);
    }

    if (y0 == y2) { // Handle awkward all-on-same-line case as its own thing
        a = b = x0;
        if (x1 < a)
            a = x1;
        else if (x1 > b)
            b = x1;
        if (x2 < a)
            a = x2;
        else if (x2 > b)
            b = x2;
        drawHLine(a, y0, b - a + 1, color);
        return;
    }

    int16_t dx01 = x1 - x0, dy01 = y1 - y0, dx02 = x2 - x0, dy02 = y2 - y0,
        dx12 = x2 - x1, dy12 = y2 - y1;
    int32_t sa = 0, sb = 0;

    // For upper part of triangle, find scanline crossings for segments
    // 0-1 and 0-2.  If y1=y2 (flat-bottomed triangle), the scanline y1
    // is included here (and second loop will be skipped, avoiding a /0
    // error there), otherwise scanline y1 is skipped here and handled
    // in the second loop...which also avoids a /0 error here if y0=y1
    // (flat-topped triangle).
    if (y1 == y2)
        last = y1; // Include y1 scanline
    else
        last = y1 - 1; // Skip it

    for (y = y0; y <= last; y++) {
        a = x0 + sa / dy01;
        b = x0 + sb / dy02;
        sa += dx01;
        sb += dx02;
        /* longhand:
        a = x0 + (x1 - x0) * (y - y0) / (y1 - y0);
        b = x0 + (x2 - x0) * (y - y0) / (y2 - y0);
        */
        if (a > b)
            swap(a, b);
        drawHLine(a, y, b - a + 1, color);
    }

    // For lower part of triangle, find scanline crossings for segments
    // 0-2 and 1-2.  This loop is skipped if y1=y2.
    sa = (int32_t)dx12 * (y - y1);
    sb = (int32_t)dx02 * (y - y0);
    for (; y <= y2; y++) {
        a = x1 + sa / dy12;
        b = x0 + sb / dy02;
        sa += dx12;
        sb += dx02;
        /* longhand:
        a = x1 + (x2 - x1) * (y - y1) / (y2 - y1);
        b = x0 + (x2 - x0) * (y - y0) / (y2 - y0);
        */
        if (a > b)
            swap(a, b);
        drawHLine(a, y, b - a + 1, color);
    }
}

void Gray2bitMap::drawMonoMapToColor(uint16_t x, uint16_t y, uint8_t *image, uint16_t width, uint16_t height, uint8_t color) {
    uint16_t widthBytes = (width + 7) / 8;
    uint16_t w0 = width >> 3;
    uint16_t w1 = width & 7;

    for (uint16_t iy = 0; iy < height; ++iy) {
        uint8_t* ptr = image + iy * widthBytes;

        uint16_t ix = x;
        for (uint16_t i = 0; i < w0; ++i) {
            uint8_t c = *ptr++;
            for (uint8_t k = 0; k < 8; ++k, ++ix) {
                if (c & 0x80)
                    setPoint(ix, y + iy, color);
                c <<= 1;
            }
        }
        uint8_t c = *ptr;
        for (uint8_t k = 0; k < w1; ++k, ++ix) {
            if (c & 0x80)
                setPoint(ix, y + iy, color);
            c <<= 1;
        }
    }
}

void Gray2bitMap::getTextBounds(const String &str, uint16_t *w, uint16_t *h) {
  *h = 16;
  *w = 0;

  for(int i=0; i<str.length(); ++i) {
    if(0x80 <= str[i]) { //---------- 한글 ---------
        *w += 16;
        i += 2;
    } else {         //---------- ASCII ---------
      *w += 8;
    }
  }
}

void Gray2bitMap::drawString(int x, int y, const String &text, Alignment align, uint8_t color) {
  int16_t  x1, y1; //the bounds of x,y and w and h of the variable 'text' in pixels.
  uint16_t w, h;
  
  getTextBounds(text, &w, &h);
  if (align == Alignment::RIGHT)  
    x = x - w;
  if (align == Alignment::CENTER) 
    x = x - w / 2;

  putString(x, y, text, color);
}