
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

bool Player::hasWon()
{
    return this->winPos > 0 ? true : false;
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

std::vector<DartThrow> Player::getLastThrows(size_t amount)
{
    std::vector<DartThrow> t;
    uint8_t n = (this->dartThrows.size() % amount);

    if(n <= 0)
        return t;

    if(this->dartThrows.size() <= n)
        return this->dartThrows;

    for(size_t i = this->dartThrows.size() - n; i < this->dartThrows.size(); i++)
        t.push_back(this->dartThrows.at(i));

    return t;
}

void Player::setCode(String code)
{
    this->code = code;
}

void Player::setName(String name)
{
    this->name = name;
}
