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
        uint16_t points;

    public:
        Player();
        Player(String code, String name);

        std::vector<DartThrow> dartThrows;

        void addThrow(DartThrow t);

        String getCode();
        String getName();

        void setCode(String code);
        void setName(String name);
};

#endif
