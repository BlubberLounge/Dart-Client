
#include "player.h"

class dartGame
{
    private:
        uint8_t playerCount = 0;

    protected:
        player players[4];

    public:
        dartGame();

        String listPlayers();
        void addPlayer(String code, String name);
        void removePlayer(String code);
        void resetPlayer();
};
