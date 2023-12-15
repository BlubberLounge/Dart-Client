#ifndef DartThrow_h
#define DartThrow_h

#include <Arduino.h>


class DartThrow
{
    private:
    protected:
        uint8_t leg;
        uint8_t turn;
        uint8_t num;
        uint8_t value;
        uint8_t field;
        String ring;
        float x;
        float y;

    public:
        DartThrow();

        uint8_t getLeg();
        uint8_t getTurn();
        uint8_t getNum();
        uint8_t getValue();
        uint8_t getField();
        String getRing();
        float getX();
        float getY();

        void setLeg(uint8_t leg);
        void setTurn(uint8_t turn);
        void setNum(uint8_t num);
        void setValue(uint8_t value);
        void setField(uint8_t field);
        void setRing(String ring);
        void setX(float x);
        void setY(float y);
};

#endif
