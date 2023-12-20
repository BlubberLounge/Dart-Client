#ifndef Display_h
#define Display_h

#include <Arduino.h>
#include "dartClient.h"
#include "dartGame.h"

class Display
{
    private:

    protected:

    public:
        Display();
        void setPoints(int num);
        void setPlayerIndicator(uint32_t clr);

    typedef struct PointSegment
    {
        public:
            void drawNumber(unsigned char chr, uint8_t origin_x, uint32_t color);
            void clear();
    } psegment;
    psegment points[3];

};

GLOBAL_VAR Display display;
#endif
