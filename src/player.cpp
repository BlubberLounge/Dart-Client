
#include "player.h"

/**
 *  Constructor
 */
player::player()
{
    //
}

player::player(String code, String name)
{
    this->code = code;
    this->name = name;
}

String player:: getCode()
{
    return this->code;
}

String player::getName()
{
    return this->name;
}

void player::setCode(String code)
{
    this->code = code;
}

void player::setName(String name)
{
    this->name = name;
}
