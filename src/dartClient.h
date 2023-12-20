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


#define WIFI_CONNECTED (WiFi.status() == WL_CONNECTED)

GLOBAL_VAR AsyncWebServer server _INIT_N(((80)));
GLOBAL_VAR AsyncWebSocket ws _INIT_N((("/ws")));
GLOBAL_VAR WiFiClient client;
GLOBAL_VAR DNSServer dnsServer;

GLOBAL_VAR String escapedMac _INIT("x");
GLOBAL_VAR char cmDNS[33] _INIT(MDNS_NAME);
GLOBAL_VAR bool ap_active _INIT(false);
GLOBAL_VAR StaticJsonDocument<10240> doc;

GLOBAL_VAR CRGB leds[256];

#include "Display.h"

#include "DartGame.h"
GLOBAL_VAR DartGame dart;


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

        void initStorage();
        void initConnection();
        void initDisplay();

        void handleConnection();
        void handleWeb();
};

#endif
