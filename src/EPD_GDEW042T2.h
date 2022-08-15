#ifndef _EPD_GDEW042T2_H_
#define _EPD_GDEW042T2_H_

#include <stdint.h>
#include <SPI.h>

class EPD_GDEW042T2 {
public:
    EPD_GDEW042T2(int16_t sck, int16_t miso, int16_t mosi, int16_t cs, int16_t dc, int16_t rst, int16_t busy);
    ~EPD_GDEW042T2() {}

    void init(void);
    void clear(uint8_t color = 0xFF);
    void display(uint8_t *image);
    void sleep(void);
    void partialDisplay(uint16_t X_start, uint16_t Y_start, uint16_t X_end, uint16_t Y_end, uint8_t *image);

    void init_4Gray(void);
    void grayDisplay(uint8_t *image);
    void grayDisplayPGM(const uint8_t *image);
    void grayDisplayPGM(const uint8_t *imageMSB, const uint8_t *imageLSB);

public:    
private: 
    static constexpr uint16_t WIDTH = 400;
    static constexpr uint16_t HEIGHT = 300;
    static constexpr uint16_t PixelCount = static_cast<uint16_t>(WIDTH*HEIGHT);

    void _init();
    void _waitBusy();
    void _reset();
    void _command(uint8_t cmd);
    void _data(uint8_t data);
    void _dataFill(uint8_t data, uint16_t n);
    void _data(const uint8_t* data, uint16_t n);
    void _dataPGM(const uint8_t* data, uint16_t n);
    void _turnOnDisplay();
    void _setFullLUT();
    void _setPartialLUT();
    void _set4GrayLUT();

    const int16_t SCK_PIN_, MISO_PIN_, MOSI_PIN_, CS_PIN_, DC_PIN_, RST_PIN_, BUSY_PIN_;
    const SPISettings spi_settings_;

    static constexpr int16_t reset_duration_ = 100;
    static constexpr int16_t refresh_duration_ = 100;
    static constexpr int16_t busy_wait_ = 10;
};

#endif//_EPD_GDEW042T2_H_