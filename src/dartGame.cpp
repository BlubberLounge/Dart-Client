
#include "dartGame.h"
#include "player.h"
#include "const.h"

/**
 *  Constructor
 */
dartGame::dartGame()
{
    //
}

String dartGame::listPlayers()
{
    String res = "";

    for(player &p : this->players)
    {
        res += "\nPlayer =>";
        res += "\n\tCode: " + p.getCode();
        res +="\n\tName: "+ p.getName();
    }

    res += "\n\nPlayerCount: ";
    res += (String)this->playerCount;

    return res;
}

void dartGame::addPlayer(String code, String name)
{
    if(playerCount+1 >= MAX_DARTPLAYER)
        return;

    player &p = this->players[playerCount];
    p.setCode(code);
    p.setName(name);
    p.setActive(true);

    this->playerCount++;
}

void dartGame::removePlayer(String code)
{

    for(player &p : this->players)
    {
        if(p.getCode() == code) {
            p.setCode("");
            p.setName("");
            p.setActive(false);
            this->playerCount--;
        }
    }
}
