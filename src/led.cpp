#include "dartClient.h"
#include "Display.h"

int getPixelIndexByXY(uint8_t x, uint8_t y)
{
    int index = 0;

    if(y % 2)
        x = 15 - x;

    if(y != 0)
        index = y * 16;

    index += x;

    return index;
}

void setPixel(int index)
{
    leds[index] = CRGB::Red;
}

void setPixelXY(uint8_t x, uint8_t y, uint32_t clr)
{
    leds[getPixelIndexByXY(x, y)] = CRGB(clr);
}

void setPixelBri(uint8_t x, uint8_t y, uint8_t bri)
{
    leds[getPixelIndexByXY(x, y)].fadeLightBy( bri );
}

