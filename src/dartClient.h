#ifndef dartClient_h
#define dartClient_h

// GLOBAL VARIABLES
// both declared and defined in header (solution from http://www.keil.com/support/docs/1868.htm)
//
//e.g. byte test = 2 becomes GLOBAL_VAR byte test _INIT(2);
//     int arr[]{0,1,2} becomes GLOBAL_VAR int arr[] _INIT_N(({0,1,2}));

#ifndef DEFINE_GLOBAL_VARS
    # define GLOBAL_VAR extern
    # define _INIT(x)
    # define _INIT_N(x)
#else
    # define GLOBAL_VAR
    # define _INIT(x) = x

    //needed to ignore commas in array definitions
    #define UNPACK( ... ) __VA_ARGS__
    # define _INIT_N(x) UNPACK x
#endif

#include "const.h"
#include "fnc.h"
#include "config.h"
#include "defaults.h"

GLOBAL_VAR ESP8266WebServer server _INIT_N(((80)));

class dartClient
{
    // private:

    // protected:
    //     void registerWebRoutes();

    public:
        dartClient();
        static dartClient& instance()
        {
            // Singleton architecture
            static dartClient instance;
            return instance;
        }

        void init();
        void loop();

        void handleConnection();
        void handleWeb();
};

#endif