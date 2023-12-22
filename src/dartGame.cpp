
#include "DartGame.h"
#include "Player.h"
#include "const.h"
#include "Display.h"

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

    display.setPlayerIndicator(this->currentPlayer->getColor());
    display.setPoints((this->points - this->currentPlayer->getPoints()));
    display.setThrowIndicator(3);
}

bool DartGame::addThrow(DartThrow t)
{
    if((this->points - this->currentPlayer->getPoints()) < t.getValue()) {
        this->throwCounter = 0;
        this->nextPlayer();
        return false;
    }

    this->currentPlayer->addThrow(t);

    display.setPoints((this->points - this->currentPlayer->getPoints()));

    this->throwCounter++;
    display.setThrowIndicator(3-this->throwCounter);

    if(this->currentPlayer->getPoints() == this->points) {
        this->winCount++;
        this->currentPlayer->setWinPos(winCount);
    }

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
        for(DartThrow dt : p.getLastThrows(3)) {
            JsonObject t = dartThrows.createNestedObject();
            t[F("v")] = dt.getValue();
        }
    }
}

bool DartGame::deserialize(JsonObject json)
{
    bool stateResponse = json[F("d")] | false;

    // serializeJson(json, Serial);
    if(json.containsKey("gi")) {
        this->points = json[F("p")];
        this->setStatus(DartGameStatus::started);
    }

    if(json.containsKey("sc")) {
        if(json[F("sc")] == "abort")
            this->setStatus(DartGameStatus::aborted);
    }

    if(json.containsKey("ap")) {
        String code = json["ap"]["c"];
        String name = json["ap"]["n"];
        String clr = json["ap"]["clr"];
        uint32_t color;

        if(clr == "r"){
            color = CRGB::Red;
        } else if(clr == "g") {
            color = CRGB::Green;
        } else if(clr == "o") {
            color = CRGB::Orange;
        } else if(clr == "b") {
            color = CRGB::Blue;
        } else if(clr == "p") {
            color = CRGB::Magenta;
        } else {
            color = CRGB::Black;
        }

        // verify IEC7064 code
        // server-side validation

        this->addPlayer(code, name, color);
    }

    if(json.containsKey("th") && (this->getStatus() == DartGameStatus::started || this->getStatus() == DartGameStatus::running)) {
        if(this->getStatus() != DartGameStatus::running)
            this->setStatus(DartGameStatus::running);

        DartThrow t;
        t.setValue(json["th"]["value"]);
        dart.addThrow(t);
    }

    return stateResponse;
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

void DartGame::addPlayer(String code, String name, uint32_t color)
{
    if(this->players.size() >= MAX_DARTPLAYER)
        return;

    Player newPlayer(code, name, color);
    this->players.push_back(newPlayer);

    if(this->players.size() == 1)
        this->setStatus(DartGameStatus::created);
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
    if(DartGameStatus::started == status) {
        this->currentPlayer = &this->players.front();

        display.setPlayerIndicator(this->currentPlayer->getColor());
        display.setPoints((this->points - this->currentPlayer->getPoints()));
        display.setThrowIndicator(3);
    } else if(DartGameStatus::aborted == status || DartGameStatus::done == status ) {
        this->reset();
    }

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

void DartGame::reset()
{
    display.setPlayerIndicator(CRGB::Black);
    display.setPoints(000);
    display.setThrowIndicator(0);

    this->currentPlayer = nullptr;
    this->players.clear();
    this->currentPlayerIndex = 0;
    this->points = 301;
    this->throwCounter = 0;
    this->winCount = 0;
    this->setStatus(DartGameStatus::unkown);
}
