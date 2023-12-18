#ifndef Player_h
#define Player_h

#include <Arduino.h>
#include "DartThrow.h"


class Player
{
    private:

    protected:
        String code;
        String name;
        uint8_t winPos = 0;
        std::vector<DartThrow> dartThrows;


    public:
        Player();
        Player(String code, String name);

        void addThrow(DartThrow t);

        bool hasWon();

        String getCode();
        String getName();
        uint16_t getPoints();
        std::vector<DartThrow> getLastThrows(size_t amount);

        void setCode(String code);
        void setName(String name);
};

#endif
