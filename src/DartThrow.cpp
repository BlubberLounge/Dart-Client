
#include "DartThrow.h"

/**
 *  Constructor
 */
DartThrow::DartThrow()
{
    //
}

uint8_t DartThrow::getValue()
{
    return this->value;
}

void DartThrow::setLeg(uint8_t leg)
{
    this->leg = leg;
}

void DartThrow::setTurn(uint8_t turn)
{
    this->turn = turn;
}

void DartThrow::setNum(uint8_t num)
{
    this->num = num;
}

void DartThrow::setValue(uint8_t value)
{
    this->value = value;
}

void DartThrow::setField(uint8_t field)
{
    this->field = field;
}

void DartThrow::setRing(String ring)
{
    this->ring = ring;
}

void DartThrow::setX(float x)
{
    this->x = x;
}

void DartThrow::setY(float y)
{
    this->y = y;
}
