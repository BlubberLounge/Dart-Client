#ifndef dartGame_h
#define dartGame_h

#include <ArduinoJson.h>
#include "DartThrow.h"
#include "Player.h"

enum class DartGameStatus
{
    unkown,
    created,
    started,
    running,
    done,
    aborted,
    error
};

class DartGame
{
    private:
        void nextPlayer();

    protected:
        std::vector<Player> players;
        DartGameStatus status = DartGameStatus::unkown;

        Player *currentPlayer = nullptr;
        uint8_t currentPlayerIndex = 0; //use a pointer at a later point
        uint8_t throwCounter = 0;
        std::size_t winCount = 0;
        const uint16_t points = 301;
        uint8_t turn = 0;   // current turn

    public:
        DartGame();

        bool addThrow(DartThrow t);
        bool isDone();

        void serialize(JsonObject j);
        bool deserialize(JsonObject j);
        void loadFromFile(String path);
        void saveToFile(String path);

        String listPlayers();
        void addPlayer(String code, String name);
        void removePlayer(String code);
        void resetPlayer();
        // void setCurrentPlayer(Player &player);
        // Player getCurrentPlayer();

        DartGameStatus getStatus();
        void setStatus(DartGameStatus status);
        String getStatusString();
};

#endif
