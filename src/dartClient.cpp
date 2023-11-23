#define DEFINE_GLOBAL_VARS //only in one source file, wled.cpp!
#include "dartClient.h"

#include <Arduino.h>


/**
 *  Constructor 
 */
dartClient::dartClient()
{
    //
}

void dartClient::init()
{
    Serial.begin(115200);
    
    handleConnection();

    // initilaize all web server routes
    initServer();
}

void dartClient::loop()
{
    //
}

void dartClient::handleConnection()
{
    #ifdef USE_WIFI
        WiFi.hostname(WIFI_HOSTNAME);
        WiFi.softAPdisconnect(true);
        WiFi.mode(WIFI_STA);
        WiFi.begin(WIFI_SSID, WIFI_PWD);
        if (WiFi.waitForConnectResult() != WL_CONNECTED) {
            Serial.println("WiFi Connect Failed! Starting in Offline mode...");
            delay(1000);
            // ESP.restart();
        }
        
        Serial.print("Open http://");
        Serial.print(WiFi.localIP());
    #endif

}