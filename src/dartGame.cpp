
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

    for(player &p : this->players) {
        res += "\nPlayer =>";
        res += "\n\tCode: " + p.getCode();
        res +="\n\tName: "+ p.getName();
    }

    res += "\n\nPlayerCount: ";
    res += (String)this->players.size();

    return res;
}

void dartGame::addPlayer(String code, String name)
{
    if(this->players.size() >= MAX_DARTPLAYER)
        return;

    player newPlayer(code, name);
    this->players.push_back(newPlayer);
}

void dartGame::removePlayer(String code)
{
    for(std::size_t i = 0; i < this->players.size(); i++)
        if(this->players[i].getCode() == code)
            this->players.erase(this->players.begin()+i);
}

dartGameStatus dartGame::getStatus()
{
    return this->status;
}

void dartGame::setStatus(dartGameStatus status)
{
    this->status = status;
}
