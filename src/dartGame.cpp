
#include "DartGame.h"
#include "Player.h"
#include "const.h"

/**
 *  Constructor
 */
DartGame::DartGame()
{
    //
}

void DartGame::nextPlayer()
{
    this->currentPlayer = this->players.at(this->currentPlayerIndex + 1);
}

bool DartGame::addThrow(DartThrow t)
{
    this->currentPlayer.addThrow(t);
    this->throwCounter++;

    if(this->throwCounter >= 3) {
        this->throwCounter = 0;
        this->nextPlayer();
    }

    return true;
}

void DartGame::serialize(JsonObject j)
{
    j[F("points")] = this->points;
    j[F("state")] = this->getStatusString();
    j[F("currentPlayer")] = this->currentPlayer.getCode();

    JsonArray players = j.createNestedArray(F("players"));
    for(Player p : this->players) {
        JsonObject player = players.createNestedObject();
        player[F("code")] = p.getCode();
        player[F("name")] = p.getName();

        JsonArray dartThrows = player.createNestedArray(F("throws"));
        for(DartThrow dt : p.dartThrows) {
            JsonObject t = dartThrows.createNestedObject();
            t[F("value")] = dt.getValue();
        }
    }
}

void DartGame::deserialize(JsonObject json)
{
    // TODO
}

void DartGame::loadFromFile(String path)
{
    // TODO
}

void DartGame::saveToFile(String path)
{
    // TODO
}

String DartGame::listPlayers()
{
    String res = "";

    for(Player &p : this->players) {
        res += "\nPlayer =>";
        res += "\n\tCode: " + p.getCode();
        res +="\n\tName: "+ p.getName();
    }

    res += "\n\nPlayerCount: ";
    res += (String)this->players.size();

    return res;
}

void DartGame::addPlayer(String code, String name)
{
    if(this->players.size() >= MAX_DARTPLAYER)
        return;

    Player newPlayer(code, name);
    this->players.push_back(newPlayer);
}

void DartGame::removePlayer(String code)
{
    for(std::size_t i = 0; i < this->players.size(); i++)
        if(this->players[i].getCode() == code)
            this->players.erase(this->players.begin()+i);
}

DartGameStatus DartGame::getStatus()
{
    return this->status;
}

void DartGame::setStatus(DartGameStatus status)
{
    if(status == DartGameStatus::created)
        this->currentPlayer = this->players.front();

    this->status = status;
}


String DartGame::getStatusString()
{
    switch(this->getStatus()) {
        case DartGameStatus::unkown: return "unkown"; break;
        case DartGameStatus::created: return "created"; break;
        case DartGameStatus::started: return "started"; break;
        case DartGameStatus::running: return "running"; break;
        case DartGameStatus::done: return "done"; break;
        case DartGameStatus::aborted: return "aborted"; break;
        case DartGameStatus::error: return "error"; break;
        default: return "unkown"; break;
    }
}

void DartGame::setCurrentPlayer(Player &player)
{
    this->currentPlayer = player;
}

Player DartGame::getCurrentPlayer()
{
    return this->currentPlayer;
}
