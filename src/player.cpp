
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

void Player::setCode(String code)
{
    this->code = code;
}

void Player::setName(String name)
{
    this->name = name;
}
