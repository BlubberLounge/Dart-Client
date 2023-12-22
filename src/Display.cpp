
#include "Display.h"
#include <Arduino.h>

#include "mapping_numbers.h"

/**
 *  Constructor
 */
Display::Display()
{
    //
}

void Display::service()
{
    bool show = false;

    if(millis() - this->lastShow > 100)
        show = true;

    if(show) {
        this->lastShow = millis();
        yield();
        FastLED.show();
    }

}

void Display::PointSegment::drawNumber(unsigned char chr, uint8_t origin_x, uint32_t color)
{
    if (chr < 48 || chr > 57) return; // only ASCII 0-9 numbers supported
    chr -= 48;
    CRGB col = CRGB(color);

    for (int i = 0; i < 9; i++) {
        uint8_t bits = pgm_read_byte_near(&mapping_numbers[(chr * 9) + i]);
        for (int j = 0; j < 4; j++) {
            int x0 = origin_x + (4-1) - j;
            if (((bits>>(j+(8-4))) & 0x01)) {
                setPixel(getPixelIndexByXY(x0, i));
            }
        }
    }
}

void Display::setPoints(int num)
{
    if(num > 999)
        return;

    uint8_t offset = 0;
    if(num < 10) {
        offset = 6;
    } else if(num < 100) {
        offset = 3;
    }

    this->clearPoints();

    const uint8_t width = 4;
    const uint8_t space = 2;
    char chrs[3];
    // numbers to char array
    itoa(num, chrs, 10);

    for(uint8_t i = 0; i < (sizeof(chrs) / sizeof(chrs[0])); i++)
        this->points[i].drawNumber(chrs[i], ((width + space) * i) + offset, 0xffffff);
}

void Display::setPlayerIndicator(uint32_t clr)
{
    for(int i = 5; i < 11; i++) {
        setPixelXY(i, 15, clr);
        setPixelXY(i, 15-1, clr);
        setPixelXY(i, 15-2, clr);
        setPixelXY(i, 15-3, clr);

        setPixelBri(i, 15, 64);
        setPixelBri(i, 15-1, 64);
        setPixelBri(i, 15-2, 64);
        setPixelBri(i, 15-3, 64);
    }

    setPixelXY(5, 15, CRGB::Black);
    setPixelXY(10, 15, CRGB::Black);
    setPixelXY(5, 12, CRGB::Black);
    setPixelXY(10, 12, CRGB::Black);
}

void Display::setThrowIndicator(uint8_t num)
{
    if(num > 3) // 3 or less
        return;

    for (int i = 0; i < 3; i++)
        setPixelXY(14, 11 + i*2, CRGB::Black);

    for (int i = 0; i < num; i++)
        setPixelXY(14, 11 + i*2, CRGB::BlueViolet);
}


void Display::clearPoints()
{
    for (int i = 0; i < 16; i++)
        for (int j = 0; j < 9; j++)
            setPixelXY(i, j, CRGB::Black);
}

