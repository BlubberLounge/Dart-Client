#ifndef dartClient_h
#define dartClient_h

#include "const.h"
#include "config.h"
#include "defaults.h"


class dartClient
{
    // private:

    // protected:
    //     void registerWebRoutes();

    public:
        dartClient();
        static dartClient& instance()
        {
            static dartClient instance;
            return instance;
        }

        void init();
        void loop();

        void handleConnection();
        void handleWeb();
};

#endif