#ifndef player_h
#define player_h

#include <Arduino.h>


class player
{
    private:

    protected:
        String code;
        String name;
        bool active = false;

    public:
        player();
        player(String code, String name);

        String getCode();
        String getName();
        bool isActive();

        void setCode(String code);
        void setName(String name);
        void setActive(bool isActive);
};

#endif
