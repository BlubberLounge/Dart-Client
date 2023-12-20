
#include "Display.h"
#include <Arduino.h>

#include "mapping_numbers.h"

/**
 *  Constructor
 */
Display::Display()
{
    // get pixel indexes
    for(uint8_t i = 0; i < (sizeof(this->points) / sizeof(points[0])); i++)
    {
        uint8_t offset = 6 * i;

        // 1
        this->points[i].n[0] = getPixelIndexByXY(1 + offset, 8);
        this->points[i].n[1] = getPixelIndexByXY(2 + offset, 8);

        // 2
        this->points[i].nw[0] = getPixelIndexByXY(0 + offset, 5);
        this->points[i].nw[1] = getPixelIndexByXY(0 + offset, 6);
        this->points[i].nw[2] = getPixelIndexByXY(0 + offset, 7);

        // 3
        this->points[i].ne[0] = getPixelIndexByXY(3 + offset, 5);
        this->points[i].ne[1] = getPixelIndexByXY(3 + offset, 6);
        this->points[i].ne[2] = getPixelIndexByXY(3 + offset, 7);

        // 4
        this->points[i].c[0] = getPixelIndexByXY(1 + offset, 4);
        this->points[i].c[1] = getPixelIndexByXY(2 + offset, 4);

        // 5
        this->points[i].sw[0] = getPixelIndexByXY(0 + offset, 1);
        this->points[i].sw[1] = getPixelIndexByXY(0 + offset, 2);
        this->points[i].sw[2] = getPixelIndexByXY(0 + offset, 3);

        // 6
        this->points[i].se[0] = getPixelIndexByXY(3 + offset, 1);
        this->points[i].se[1] = getPixelIndexByXY(3 + offset, 2);
        this->points[i].se[2] = getPixelIndexByXY(3 + offset, 3);

        // 7
        this->points[i].s[0] = getPixelIndexByXY(1 + offset, 0);
        this->points[i].s[1] = getPixelIndexByXY(2 + offset, 0);
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
    FastLED.clear();
    if(num > 999)
        return;

    uint8_t offset = 0;
    if(num < 100)
        offset = 3;

    const uint8_t width = 4;
    const uint8_t space = 2;
    char chrs[3];
    // numbers to char array
    itoa(num, chrs, 10);

    for(uint8_t i = 0; i < (sizeof(chrs) / sizeof(chrs[0])); i++)
        this->points[i].drawNumber(chrs[i], ((width + space) * i) + offset, 0xffffff);

    delay(1);
    FastLED.show();
    FastLED.show();
}

