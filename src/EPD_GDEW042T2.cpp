#include "EPD_GDEW042T2.h"

#include <Arduino.h>

#if defined(ESP8266) || defined(ESP32)
#include <pgmspace.h>
#else
#include <avr/pgmspace.h>
#endif


static const uint8_t EPD_4IN2_lut_vcom0[] PROGMEM = {
    0x00, 0x17, 0x00, 0x00, 0x00, 0x02,
    0x00, 0x17, 0x17, 0x00, 0x00, 0x02,
    0x00, 0x0A, 0x01, 0x00, 0x00, 0x01,
    0x00, 0x0E, 0x0E, 0x00, 0x00, 0x02,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
};
static const uint8_t EPD_4IN2_lut_ww[] PROGMEM = {
    0x40, 0x17, 0x00, 0x00, 0x00, 0x02,
    0x90, 0x17, 0x17, 0x00, 0x00, 0x02,
    0x40, 0x0A, 0x01, 0x00, 0x00, 0x01,
    0xA0, 0x0E, 0x0E, 0x00, 0x00, 0x02,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
};
static const uint8_t EPD_4IN2_lut_bw[] PROGMEM = {
    0x40, 0x17, 0x00, 0x00, 0x00, 0x02,
    0x90, 0x17, 0x17, 0x00, 0x00, 0x02,
    0x40, 0x0A, 0x01, 0x00, 0x00, 0x01,
    0xA0, 0x0E, 0x0E, 0x00, 0x00, 0x02,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
};
static const uint8_t EPD_4IN2_lut_wb[] PROGMEM = {
    0x80, 0x17, 0x00, 0x00, 0x00, 0x02,
    0x90, 0x17, 0x17, 0x00, 0x00, 0x02,
    0x80, 0x0A, 0x01, 0x00, 0x00, 0x01,
    0x50, 0x0E, 0x0E, 0x00, 0x00, 0x02,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
};
static const uint8_t EPD_4IN2_lut_bb[] PROGMEM = {
    0x80, 0x17, 0x00, 0x00, 0x00, 0x02,
    0x90, 0x17, 0x17, 0x00, 0x00, 0x02,
    0x80, 0x0A, 0x01, 0x00, 0x00, 0x01,
    0x50, 0x0E, 0x0E, 0x00, 0x00, 0x02,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
};

/******************************partial screen update LUT*********************************/
const uint8_t EPD_4IN2_Partial_lut_vcom1[] PROGMEM = {
0x00	,0x19	,0x01	,0x00	,0x00	,0x01,
0x00	,0x00	,0x00	,0x00	,0x00	,0x00,
0x00	,0x00	,0x00	,0x00	,0x00	,0x00,
0x00	,0x00	,0x00	,0x00	,0x00	,0x00,
0x00	,0x00	,0x00	,0x00	,0x00	,0x00,
0x00	,0x00	,0x00	,0x00	,0x00	,0x00,
0x00	,0x00	,0x00	,0x00	,0x00	,0x00,
0x00	,0x00, };

const uint8_t EPD_4IN2_Partial_lut_ww1[] PROGMEM = {
0x00	,0x19	,0x01	,0x00	,0x00	,0x01,
0x00	,0x00	,0x00	,0x00	,0x00	,0x00,
0x00	,0x00	,0x00	,0x00	,0x00	,0x00,
0x00	,0x00	,0x00	,0x00	,0x00	,0x00,
0x00	,0x00	,0x00	,0x00	,0x00	,0x00,
0x00	,0x00	,0x00	,0x00	,0x00	,0x00,
0x00	,0x00	,0x00	,0x00	,0x00	,0x00, };

const uint8_t EPD_4IN2_Partial_lut_bw1[] PROGMEM = {
0x80	,0x19	,0x01	,0x00	,0x00	,0x01,
0x00	,0x00	,0x00	,0x00	,0x00	,0x00,
0x00	,0x00	,0x00	,0x00	,0x00	,0x00,
0x00	,0x00	,0x00	,0x00	,0x00	,0x00,
0x00	,0x00	,0x00	,0x00	,0x00	,0x00,
0x00	,0x00	,0x00	,0x00	,0x00	,0x00,
0x00	,0x00	,0x00	,0x00	,0x00	,0x00, };

const uint8_t EPD_4IN2_Partial_lut_wb1[] PROGMEM = {
0x40	,0x19	,0x01	,0x00	,0x00	,0x01,
0x00	,0x00	,0x00	,0x00	,0x00	,0x00,
0x00	,0x00	,0x00	,0x00	,0x00	,0x00,
0x00	,0x00	,0x00	,0x00	,0x00	,0x00,
0x00	,0x00	,0x00	,0x00	,0x00	,0x00,
0x00	,0x00	,0x00	,0x00	,0x00	,0x00,
0x00	,0x00	,0x00	,0x00	,0x00	,0x00, };

const uint8_t EPD_4IN2_Partial_lut_bb1[] PROGMEM = {
0x00	,0x19	,0x01	,0x00	,0x00	,0x01,
0x00	,0x00	,0x00	,0x00	,0x00	,0x00,
0x00	,0x00	,0x00	,0x00	,0x00	,0x00,
0x00	,0x00	,0x00	,0x00	,0x00	,0x00,
0x00	,0x00	,0x00	,0x00	,0x00	,0x00,
0x00	,0x00	,0x00	,0x00	,0x00	,0x00,
0x00	,0x00	,0x00	,0x00	,0x00	,0x00, };

/******************************gray*********************************/
//0~3 gray
const uint8_t EPD_4IN2_4Gray_lut_vcom[] PROGMEM = {
0x00	,0x0A	,0x00	,0x00	,0x00	,0x01,
0x60	,0x14	,0x14	,0x00	,0x00	,0x01,
0x00	,0x14	,0x00	,0x00	,0x00	,0x01,
0x00	,0x13	,0x0A	,0x01	,0x00	,0x01,
0x00	,0x00	,0x00	,0x00	,0x00	,0x00,
0x00	,0x00	,0x00	,0x00	,0x00	,0x00,
0x00	,0x00	,0x00	,0x00	,0x00	,0x00
};
//R21
const uint8_t EPD_4IN2_4Gray_lut_ww[] PROGMEM = {
0x40	,0x0A	,0x00	,0x00	,0x00	,0x01,
0x90	,0x14	,0x14	,0x00	,0x00	,0x01,
0x10	,0x14	,0x0A	,0x00	,0x00	,0x01,
0xA0	,0x13	,0x01	,0x00	,0x00	,0x01,
0x00	,0x00	,0x00	,0x00	,0x00	,0x00,
0x00	,0x00	,0x00	,0x00	,0x00	,0x00,
0x00	,0x00	,0x00	,0x00	,0x00	,0x00,
};
//R22H	r
const uint8_t EPD_4IN2_4Gray_lut_bw[] PROGMEM = {
0x40	,0x0A	,0x00	,0x00	,0x00	,0x01,
0x90	,0x14	,0x14	,0x00	,0x00	,0x01,
0x00	,0x14	,0x0A	,0x00	,0x00	,0x01,
0x99	,0x0C	,0x01	,0x03	,0x04	,0x01,
0x00	,0x00	,0x00	,0x00	,0x00	,0x00,
0x00	,0x00	,0x00	,0x00	,0x00	,0x00,
0x00	,0x00	,0x00	,0x00	,0x00	,0x00,
};
//R23H	w
const uint8_t EPD_4IN2_4Gray_lut_wb[] PROGMEM = {
0x40	,0x0A	,0x00	,0x00	,0x00	,0x01,
0x90	,0x14	,0x14	,0x00	,0x00	,0x01,
0x00	,0x14	,0x0A	,0x00	,0x00	,0x01,
0x99	,0x0B	,0x04	,0x04	,0x01	,0x01,
0x00	,0x00	,0x00	,0x00	,0x00	,0x00,
0x00	,0x00	,0x00	,0x00	,0x00	,0x00,
0x00	,0x00	,0x00	,0x00	,0x00	,0x00,
};
//R24H	b
const uint8_t EPD_4IN2_4Gray_lut_bb[] PROGMEM = {
0x80	,0x0A	,0x00	,0x00	,0x00	,0x01,
0x90	,0x14	,0x14	,0x00	,0x00	,0x01,
0x20	,0x14	,0x0A	,0x00	,0x00	,0x01,
0x50	,0x13	,0x01	,0x00	,0x00	,0x01,
0x00	,0x00	,0x00	,0x00	,0x00	,0x00,
0x00	,0x00	,0x00	,0x00	,0x00	,0x00,
0x00	,0x00	,0x00	,0x00	,0x00	,0x00,
};

EPD_GDEW042T2::EPD_GDEW042T2(int16_t sck, int16_t miso, int16_t mosi, int16_t cs, int16_t dc, int16_t rst, int16_t busy) :
    SCK_PIN_(sck), MISO_PIN_(miso), MOSI_PIN_(mosi), CS_PIN_(cs), DC_PIN_(dc), RST_PIN_(rst), BUSY_PIN_(busy), spi_settings_(4000000, MSBFIRST, SPI_MODE0)
{
    SPI.end();
    SPI.begin(SCK_PIN_, MISO_PIN_, MOSI_PIN_, CS_PIN_);

    pinMode(BUSY_PIN_, INPUT);

    digitalWrite(CS_PIN_, HIGH);
    pinMode(CS_PIN_, OUTPUT);

    digitalWrite(DC_PIN_, HIGH);
    pinMode(DC_PIN_, OUTPUT);

    digitalWrite(RST_PIN_, HIGH);
    pinMode(RST_PIN_, OUTPUT);
}

void EPD_GDEW042T2::_init() {

}

void EPD_GDEW042T2::_waitBusy() {
    while (digitalRead(BUSY_PIN_) == LOW)
        delay(busy_wait_);
}

void EPD_GDEW042T2::_reset() {
    digitalWrite(RST_PIN_, HIGH);
    delay(reset_duration_);
    digitalWrite(RST_PIN_, LOW);
    delay(reset_duration_);
    digitalWrite(RST_PIN_, HIGH);
    delay(reset_duration_);
}


void EPD_GDEW042T2::_command(uint8_t cmd) {
    _waitBusy();
    SPI.beginTransaction(spi_settings_);
    digitalWrite(DC_PIN_, LOW);
    digitalWrite(CS_PIN_, LOW);
    SPI.transfer(cmd);
    digitalWrite(CS_PIN_, HIGH);
    SPI.endTransaction();
}

void EPD_GDEW042T2::_data(uint8_t data) {
    _waitBusy();

    SPI.beginTransaction(spi_settings_);
    digitalWrite(DC_PIN_, HIGH);
    digitalWrite(CS_PIN_, LOW);

    SPI.transfer(data);

    digitalWrite(CS_PIN_, HIGH);
    SPI.endTransaction();
}

void EPD_GDEW042T2::_dataFill(uint8_t data, uint16_t n)
{
    _waitBusy();

    SPI.beginTransaction(spi_settings_);
    digitalWrite(DC_PIN_, HIGH);
    digitalWrite(CS_PIN_, LOW);

    while (n--) SPI.transfer(data);

    digitalWrite(CS_PIN_, HIGH);
    SPI.endTransaction();
}

void EPD_GDEW042T2::_data(const uint8_t* data, uint16_t n) {
    _waitBusy();

    SPI.beginTransaction(spi_settings_);
    digitalWrite(DC_PIN_, HIGH);
    digitalWrite(CS_PIN_, LOW);

    while (n--) SPI.transfer(*data++);

    digitalWrite(CS_PIN_, HIGH);
    SPI.endTransaction();
}

void EPD_GDEW042T2::_dataPGM(const uint8_t* data, uint16_t n) {
    _waitBusy();
    SPI.beginTransaction(spi_settings_);
    digitalWrite(DC_PIN_, HIGH);
    digitalWrite(CS_PIN_, LOW);

    while (n--) SPI.transfer(pgm_read_byte(&*data++));

    digitalWrite(CS_PIN_, HIGH);
    SPI.endTransaction();
}

void EPD_GDEW042T2::_setFullLUT() {
    _command(0x20);
    _dataPGM(EPD_4IN2_lut_vcom0, 44);
    _command(0x21);
    _dataPGM(EPD_4IN2_lut_ww, 42);
    _command(0x22);
    _dataPGM(EPD_4IN2_lut_bw, 42);
    _command(0x23);
    _dataPGM(EPD_4IN2_lut_wb, 42);
    _command(0x24);
    _dataPGM(EPD_4IN2_lut_bb, 42);
}

void EPD_GDEW042T2::_setPartialLUT() {
    _command(0x20);
    _dataPGM(EPD_4IN2_Partial_lut_vcom1, 44);
    _command(0x21);
    _dataPGM(EPD_4IN2_Partial_lut_ww1, 42);
    _command(0x22);
    _dataPGM(EPD_4IN2_Partial_lut_bw1, 42);
    _command(0x23);
    _dataPGM(EPD_4IN2_Partial_lut_wb1, 42);
    _command(0x24);
    _dataPGM(EPD_4IN2_Partial_lut_bb1, 42);
}

void EPD_GDEW042T2::_set4GrayLUT() {
    _command(0x20);
    _dataPGM(EPD_4IN2_4Gray_lut_vcom, 44);
    _command(0x21);
    _dataPGM(EPD_4IN2_4Gray_lut_ww, 42);
    _command(0x22);
    _dataPGM(EPD_4IN2_4Gray_lut_bw, 42);
    _command(0x23);
    _dataPGM(EPD_4IN2_4Gray_lut_wb, 42);
    _command(0x24);
    _dataPGM(EPD_4IN2_4Gray_lut_bb, 42);
    _command(0x25);
    _dataPGM(EPD_4IN2_4Gray_lut_ww, 42);
}


void EPD_GDEW042T2::init(void) {
    _reset();

    _command(0x01); // POWER SETTING
    _data(0x03);
    _data(0x00);
    _data(0x2b);
    _data(0x2b);

    _command(0x06); // boost soft start
    _data(0x17);		//A
    _data(0x17);		//B
    _data(0x17);		//C

    _command(0x04); // POWER_ON
    _waitBusy();

    _command(0x00); // panel setting
    _data(0xbf); // KW-BF   KWR-AF	BWROTP 0f	BWOTP 1f
    _data(0x0d);

    _command(0x30); // PLL setting
    _data(0x3C); // 3A 100HZ   29 150Hz 39 200HZ	31 171HZ

    _command(0x61); // resolution setting
    _data(0x01);
    _data(0x90); //128
    _data(0x01); //
    _data(0x2c);

    _command(0x82); // vcom_DC setting
    _data(0x28);

    _command(0X50); // VCOM AND DATA INTERVAL SETTING
    _data(0x97); // 97white border 77black border		VBDF 17|D7 VBDW 97 VBDB 57		VBDF F7 VBDW 77 VBDB 37  VBDR B7

    _setFullLUT();
}

void EPD_GDEW042T2::clear(uint8_t color) {
    color &= 3;
    color |= (color << 2) | (color << 4) | (color << 6);

    uint16_t n = (WIDTH / 8) * HEIGHT;

    _command(0x10);
    _dataFill(color, n);

    _command(0x13);
    _dataFill(color, n);

    _command(0x12);		 //DISPLAY REFRESH 
    delay(refresh_duration_);

    _turnOnDisplay();
}

void EPD_GDEW042T2::_turnOnDisplay()
{
    _command(0x12);
    delay(refresh_duration_);
    _waitBusy();
}

void EPD_GDEW042T2::display(uint8_t *image) {
    uint16_t n = (WIDTH / 8) * HEIGHT;

    _command(0x13);
    _data(image, n);

    _turnOnDisplay();
}

void EPD_GDEW042T2::sleep(void) {
    _command(0x02); // POWER_OFF
    _waitBusy();
    _command(0x07); // DEEP_SLEEP
    _data(0XA5);
}

void EPD_GDEW042T2::partialDisplay(uint16_t X_start, uint16_t Y_start, uint16_t X_end, uint16_t Y_end, uint8_t *image) {
    uint16_t Width, Height;
    Width = (WIDTH % 8 == 0) ? (WIDTH / 8) : (WIDTH / 8 + 1);
    Height = HEIGHT;

    X_start = (X_start % 8 == 0) ? (X_start) : (X_start / 8 * 8 + 8);
    X_end = (X_end % 8 == 0) ? (X_end) : (X_end / 8 * 8 + 8);

    _command(0X50);
    _data(0xf7);
    delay(refresh_duration_);

    _command(0x82);			//vcom_DC setting  	
    _data(0x08);
    _command(0X50);
    _data(0x47);
    _setPartialLUT();
    _command(0x91);		//This command makes the display enter partial mode
    _command(0x90);		//resolution setting
    _data((X_start) / 256);
    _data((X_start) % 256);   //x-start    

    _data((X_end) / 256);
    _data((X_end) % 256 - 1);  //x-end

    _data(Y_start / 256);
    _data(Y_start % 256);   //y-start    

    _data(Y_end / 256);
    _data(Y_end % 256 - 1);  //y-end
    _data(0x28);

    _command(0x10);	       //writes Old data to SRAM for programming
    for (uint16_t j = 0; j < Y_end - Y_start; j++) {
        for (uint16_t i = 0; i < (X_end - X_start) / 8; i++) {
            _data(image[(Y_start + j)*Width + X_start / 8 + i]);
        }
    }
    _command(0x13);				 //writes New data to SRAM.
    for (uint16_t j = 0; j < Y_end - Y_start; j++) {
        for (uint16_t i = 0; i < (X_end - X_start) / 8; i++) {
            _data(~image[(Y_start + j)*Width + X_start / 8 + i]);
        }
    }

    _command(0x12);		 //DISPLAY REFRESH 		             
    delay(refresh_duration_);     //The delay here is necessary, 200uS at least!!!     
    _turnOnDisplay();
}

void EPD_GDEW042T2::init_4Gray(void) {
    _reset();

    _command(0x01);			//POWER SETTING
    _data(0x03);
    _data(0x00);       //VGH=20V,VGL=-20V
    _data(0x2b);		//VDH=15V															 
    _data(0x2b);		//VDL=-15V
    _data(0x13);

    _command(0x06);         //booster soft start
    _data(0x17);		//A
    _data(0x17);		//B
    _data(0x17);		//C 

    _command(0x04);
    _waitBusy();

    _command(0x00);			//panel setting
    _data(0x3f);		//KW-3f   KWR-2F	BWROTP 0f	BWOTP 1f

    _command(0x30);			//PLL setting
    _data(0x3c);      	//100hz 

    _command(0x61);			//resolution setting
    _data(0x01);		//400
    _data(0x90);
    _data(0x01);		//300
    _data(0x2c);

    _command(0x82);			//vcom_DC setting
    _data(0x12);

    _command(0X50);			//VCOM AND DATA INTERVAL SETTING			
    _data(0x97);

    _set4GrayLUT();
}

void EPD_GDEW042T2::grayDisplay(uint8_t *image) {
    /****Color display description****
          white  gray1  gray2  black
    Gray|  11     10     01     00
    0x10|  01     01     00     00
    0x13|  01     00     01     00
    *********************************/
    _command(0x10);
    uint8_t* ptr = image;
    for (int16_t iy = 0; iy < HEIGHT; ++iy) {
        for (int16_t ix = 0; ix < WIDTH / 8; ++ix) {
            int8_t _0x10 = 0;

            int8_t gray4 = *ptr++;
            if (gray4 & 0x80) _0x10 |= 0x01;
            _0x10 <<= 1;
            gray4 <<= 2;
            if (gray4 & 0x80) _0x10 |= 0x01;
            _0x10 <<= 1;
            gray4 <<= 2;
            if (gray4 & 0x80) _0x10 |= 0x01;
            _0x10 <<= 1;
            gray4 <<= 2;
            if (gray4 & 0x80) _0x10 |= 0x01;
            _0x10 <<= 1;

            gray4 = *ptr++;
            if (gray4 & 0x80) _0x10 |= 0x01;
            _0x10 <<= 1;
            gray4 <<= 2;
            if (gray4 & 0x80) _0x10 |= 0x01;
            _0x10 <<= 1;
            gray4 <<= 2;
            if (gray4 & 0x80) _0x10 |= 0x01;
            _0x10 <<= 1;
            gray4 <<= 2;
            if (gray4 & 0x80) _0x10 |= 0x01;

            _data(_0x10);
        }
    }
    // new  data
    _command(0x13);
    ptr = image;
    for (int16_t iy = 0; iy < HEIGHT; ++iy) {
        for (int16_t ix = 0; ix < WIDTH / 8; ++ix) {
            int8_t _0x13 = 0;

            int8_t gray4 = *ptr++;
            if (gray4 & 0x40) _0x13 |= 0x01;
            _0x13 <<= 1;
            gray4 <<= 2;
            if (gray4 & 0x40) _0x13 |= 0x01;
            _0x13 <<= 1;
            gray4 <<= 2;
            if (gray4 & 0x40) _0x13 |= 0x01;
            _0x13 <<= 1;
            gray4 <<= 2;
            if (gray4 & 0x40) _0x13 |= 0x01;
            _0x13 <<= 1;

            gray4 = *ptr++;
            if (gray4 & 0x40) _0x13 |= 0x01;
            _0x13 <<= 1;
            gray4 <<= 2;
            if (gray4 & 0x40) _0x13 |= 0x01;
            _0x13 <<= 1;
            gray4 <<= 2;
            if (gray4 & 0x40) _0x13 |= 0x01;
            _0x13 <<= 1;
            gray4 <<= 2;
            if (gray4 & 0x40) _0x13 |= 0x01;

            _data(_0x13);
        }
    }

    _turnOnDisplay();
}

void EPD_GDEW042T2::grayDisplayPGM(const uint8_t *image) {
    /****Color display description****
          white  gray1  gray2  black
    Gray|  11     10     01     00
    0x10|  01     01     00     00
    0x13|  01     00     01     00
    *********************************/
    _command(0x10);
    const uint8_t* ptr1 = image;
    for (int16_t iy = 0; iy < HEIGHT; ++iy) {
        for (int16_t ix = 0; ix < (WIDTH / 8); ++ix) {
            int8_t _0x10 = 0;

            int8_t gray4 = pgm_read_byte(&*ptr1++);
            if (gray4 & 0x80) _0x10 |= 0x01;
            _0x10 <<= 1;
            gray4 <<= 2;
            if (gray4 & 0x80) _0x10 |= 0x01;
            _0x10 <<= 1;
            gray4 <<= 2;
            if (gray4 & 0x80) _0x10 |= 0x01;
            _0x10 <<= 1;
            gray4 <<= 2;
            if (gray4 & 0x80) _0x10 |= 0x01;
            _0x10 <<= 1;

            gray4 = pgm_read_byte(&*ptr1++);
            if (gray4 & 0x80) _0x10 |= 0x01;
            _0x10 <<= 1;
            gray4 <<= 2;
            if (gray4 & 0x80) _0x10 |= 0x01;
            _0x10 <<= 1;
            gray4 <<= 2;
            if (gray4 & 0x80) _0x10 |= 0x01;
            _0x10 <<= 1;
            gray4 <<= 2;
            if (gray4 & 0x80) _0x10 |= 0x01;

            _data(_0x10);
        }
    }

    // new  data
    _command(0x13);
    const uint8_t* ptr2 = image;
    for (int16_t iy = 0; iy < HEIGHT; ++iy) {
        for (int16_t ix = 0; ix < (WIDTH / 8); ++ix) {
            int8_t _0x13 = 0;

            int8_t gray4 = pgm_read_byte(&*ptr2++);
            if (gray4 & 0x40) _0x13 |= 0x01;
            _0x13 <<= 1;
            gray4 <<= 2;
            if (gray4 & 0x40) _0x13 |= 0x01;
            _0x13 <<= 1;
            gray4 <<= 2;
            if (gray4 & 0x40) _0x13 |= 0x01;
            _0x13 <<= 1;
            gray4 <<= 2;
            if (gray4 & 0x40) _0x13 |= 0x01;
            _0x13 <<= 1;

            gray4 = pgm_read_byte(&*ptr2++);
            if (gray4 & 0x40) _0x13 |= 0x01;
            _0x13 <<= 1;
            gray4 <<= 2;
            if (gray4 & 0x40) _0x13 |= 0x01;
            _0x13 <<= 1;
            gray4 <<= 2;
            if (gray4 & 0x40) _0x13 |= 0x01;
            _0x13 <<= 1;
            gray4 <<= 2;
            if (gray4 & 0x40) _0x13 |= 0x01;

            _data(_0x13);
        }
    }

    _turnOnDisplay();
}

void EPD_GDEW042T2::grayDisplayPGM(const uint8_t *imageMSB, const uint8_t *imageLSB) {
    uint16_t n = (WIDTH / 8) * HEIGHT;

    _command(0x10);
    _dataPGM(imageMSB, n);
    // new  data
    _command(0x13);
    _dataPGM(imageLSB, n);

    _turnOnDisplay();
}
