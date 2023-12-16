
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
    if(this->winCount+1 >= this->players.size()) {
        this->setStatus(DartGameStatus::done);
        return;
    }

    this->currentPlayerIndex++;
    this->currentPlayerIndex = this->currentPlayerIndex % this->players.size();

    if(this->players.at(this->currentPlayerIndex).hasWon()) {
        this->nextPlayer();
        return;
    }

    this->currentPlayer = &this->players.at(this->currentPlayerIndex);
}

bool DartGame::addThrow(DartThrow t)
{
    if((this->points - this->currentPlayer->getPoints()) < t.getValue()) {
        this->throwCounter = 0;
        this->nextPlayer();
        return false;
    }

    this->currentPlayer->addThrow(t);
    this->throwCounter++;

    if(this->currentPlayer->getPoints() == this->points)
        this->winCount++;

    if(this->throwCounter >= 3 || this->currentPlayer->getPoints() == this->points) {
        this->throwCounter = 0;
        this->nextPlayer();
    }

    return true;
}

bool DartGame::isDone()
{
    return this->status == DartGameStatus::done;
}

void DartGame::serialize(JsonObject j)
{
    j[F("p")] = this->points;
    j[F("s")] = this->getStatusString();
    if(this->currentPlayer) {
        j[F("cp")] = this->currentPlayer->getCode();
    } else {
        j[F("cp")] = "";
    }

    JsonArray players = j.createNestedArray(F("players"));
    for(Player p : this->players) {
        JsonObject player = players.createNestedObject();
        player[F("c")] = p.getCode();
        player[F("n")] = p.getName();
        player[F("p")] = this->points - p.getPoints();

        JsonArray dartThrows = player.createNestedArray(F("throws"));
        for(DartThrow dt : p.dartThrows) {
            JsonObject t = dartThrows.createNestedObject();
            t[F("v")] = dt.getValue();
        }
    }
}

bool DartGame::deserialize(JsonObject json)
{
    return true;
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
        this->currentPlayer = &this->players.front();

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
