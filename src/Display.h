#ifndef Display_h
#define Display_h

#include <Arduino.h>
#include "dartClient.h"
#include "dartGame.h"

class Display
{
    private:

        struct PointSegment
        {
            void drawNumber(unsigned char chr, uint8_t origin_x, uint32_t color);
        };
    protected:

    public:
        Display();
        void setPoints(int num);
        void setPlayerIndicator(uint32_t clr);
        void setThrowIndicator(uint8_t num);

        void clearPoints();
    PointSegment points[3];

};

GLOBAL_VAR Display display;
#endif
