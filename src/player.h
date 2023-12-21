#ifndef Player_h
#define Player_h

#include <Arduino.h>
#include "DartThrow.h"


class Player
{
    private:
        struct Turn {
            DartThrow throws[3];
        };

    protected:
        String code;
        String name;
        uint32_t color;
        uint8_t winPos = 0;
        std::vector<DartThrow> dartThrows;


    public:
        Player();
        Player(String code, String name);
        Player(String code, String name, uint32_t color);

        void addThrow(DartThrow t);

        bool hasWon();

        String getCode();
        String getName();
        uint32_t getColor();
        uint16_t getPoints();
        std::vector<DartThrow> getLastThrows(size_t amount);

        void setCode(String code);
        void setName(String name);
};

#endif
