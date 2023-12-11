#ifndef dartGame_h
#define dartGame_h

#include "player.h"

enum class dartGameStatus
{
    unkown,
    created,
    started,
    running,
    done,
    aborted,
    error
};

class dartGame
{
    private:

    protected:
        std::vector<player> players;
        dartGameStatus status = dartGameStatus::unkown;

    public:
        dartGame();

        String listPlayers();
        void addPlayer(String code, String name);
        void removePlayer(String code);
        void resetPlayer();

        dartGameStatus getStatus();
        void setStatus(dartGameStatus status);
};

#endif
