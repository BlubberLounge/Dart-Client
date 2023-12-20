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

    typedef struct PointSegment
    {
        int n[2];
        int nw[3];
        int ne[3];
        int c[2];
        int sw[3];
        int se[3];
        int s[2];
        public:
            void drawNumber(unsigned char chr, uint8_t origin_x, uint32_t color);
            void clear();
    } psegment;
    psegment points[3];

};

GLOBAL_VAR Display display;
#endif
