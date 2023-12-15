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

    public:
        Player();
        Player(String code, String name);

        std::vector<DartThrow> dartThrows;

        void addThrow(DartThrow t);

        String getCode();
        String getName();
        uint16_t getPoints();

        void setCode(String code);
        void setName(String name);
};

#endif
