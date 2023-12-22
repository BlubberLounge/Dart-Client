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
        unsigned long lastShow = 0;

    public:
        Display();

        void service();

        void setPoints(int num);
        void setPlayerIndicator(uint32_t clr);
        void setThrowIndicator(uint8_t num);
        void showIntro();

        void clearPoints();
        PointSegment points[3];
};

GLOBAL_VAR Display display;
#endif
