
#include "Player.h"

/**
 *  Constructor
 */
Player::Player()
{
    //
}

Player::Player(String code, String name)
{
    this->code = code;
    this->name = name;
}

void Player::addThrow(DartThrow t)
{
    this->dartThrows.push_back(t);
}

String Player:: getCode()
{
    return this->code;
}

String Player::getName()
{
    return this->name;
}

uint16_t Player::getPoints()
{
    uint16_t sum = 0;
    for(DartThrow t : this->dartThrows)
        sum += t.getValue();

    return sum;
}

void Player::setCode(String code)
{
    this->code = code;
}

void Player::setName(String name)
{
    this->name = name;
}
