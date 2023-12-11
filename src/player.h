#ifndef player_h
#define player_h

#include <Arduino.h>


class player
{
    private:

    protected:
        String code;
        String name;

    public:
        player();
        player(String code, String name);

        String getCode();
        String getName();

        void setCode(String code);
        void setName(String name);
};

#endif
