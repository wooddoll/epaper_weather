#include "gray2bitmap.h"

#include <Arduino.h>

#if defined(ESP8266) || defined(ESP32)
#include <pgmspace.h>
#else
#include <avr/pgmspace.h>
#endif

#if 1
#include "Hangeul_Font.h"
#include "ASCII_Font.h"
#else
static const byte* H_in_font_Hangel_844 = NULL;
static const byte* H_in_font_ASCII = NULL;
#endif

#define UNICODE_HAN_BASE 0xAC00

static byte FontImage[32] = {0, } ;
static const byte *HanFontSet = H_in_font_Hangel_844 ;
static const byte *ASCIIFontSet = H_in_font_ASCII ;

static void MakeASCIIFont(int c)
{
  int i ;
  byte *pB ;
  const byte *pF ;
  
  pB = FontImage ;
  pF = ASCIIFontSet + (c * 16) ;
  for (i = 0; i < 16; i++) 
    *pB++ = pgm_read_byte(pF++) ;
}

static void MakeHanFont(uint16_t utf16)
{
  const byte cho[] = {0, 0, 0, 0, 0, 0, 0, 0, 1, 3, 3, 3, 1, 2, 4, 4, 4, 2, 1, 3, 0} ;
  const byte cho2[] = {5, 5, 5, 5, 5, 5, 5, 5, 6, 7, 7, 7, 6, 6, 7, 7, 7, 6, 6, 7, 5} ;
  const byte jong[] = {0, 2, 0, 2, 1, 2, 1, 2, 3, 0, 2, 1, 3, 3, 1, 2, 1, 3, 3, 1, 1} ;

  byte first, mid, last ;
  byte firstType, midType, lastType ;
  byte i ;
  byte *pB, *pF ;

  //Serial.println(utf16, HEX) ;
  last = utf16 % 28 ;
  utf16 /= 28 ;
  mid = (utf16 % 21) ;
  first = (utf16 / 21) ;

  if (!last) {
    firstType = cho[mid] ;
    if (first == 0 || first == 15) 
      midType = 0 ;
    else 
      midType = 1 ;
  } else {
    firstType = cho2[mid] ;
    if (first == 0 || first == 15) 
      midType = 2 ;
    else 
      midType = 3 ;
    lastType = jong[mid] ;
  }
  firstType ;
  midType ;
  memset(FontImage, 0, 32) ;

  pB = FontImage ;
  pF = (byte *)HanFontSet + ((firstType * 20) + (first + 1)) * 32 ;
  for (i = 0; i < 32; i++) 
    *(pB + i) = pgm_read_byte(pF + i) ; 

  pB = FontImage ;
  pF = (byte *)HanFontSet + ((8 * 20) + (midType * 22) + (mid + 1)) * 32 ;
  for (i = 0; i < 32; i++) 
    *(pB + i) |= pgm_read_byte(pF + i) ; 

  if (last) {
    pB = FontImage ;
    pF = (byte *)HanFontSet + ((8 * 20) + (4 * 22) + (lastType * 28) + last) * 32 ;
    for (i = 0; i < 32; i++) 
      *(pB + i) |= pgm_read_byte(pF + i) ; 
  }
}

void Gray2bitMap::putString(uint16_t x, uint16_t y, const String &str, uint8_t color)
{
  for(int i=0; i<str.length(); ++i) {
    if(0x80 <= str[i]) { //---------- 한글 ---------
      byte c1 = str[i++];
      byte c2 = str[i++];
      byte c3 = str[i];
      uint16_t utf16 = ((c1 & 0x0f) << 12) | ((c2 & 0x3f) << 6) | (c3 & 0x3f);
      utf16 -= UNICODE_HAN_BASE;
      
      MakeHanFont(utf16);
      drawMonoMapToColor(x, y, FontImage, 16, 16, color);
      x += 16;
    } else {         //---------- ASCII ---------
      MakeASCIIFont(str[i]);      
      drawMonoMapToColor(x, y, FontImage, 8, 16, color);
      x += 8;
    }
  }
}
