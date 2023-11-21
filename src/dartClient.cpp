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


    ArduinoOTA.begin();
    server.on("/", []() {
        #ifdef USE_WWW_AUTH
            if (!server.authenticate(WWW_USER, WWW_PWD)) {
                return server.requestAuthentication();
            }
        #endif

        server.send(200, "text/plain", "Login OK");
    });
    server.begin();

    Serial.print("Open http://");
    Serial.print(WiFi.localIP());
}

void dartClient::loop()
{
    ArduinoOTA.handle();
    server.handleClient();
}

void dartClient::handleConnection()
{
    #ifdef USE_WIFI
        WiFi.mode(WIFI_STA);
        WiFi.begin(WIFI_SSID, WIFI_PWD);
        if (WiFi.waitForConnectResult() != WL_CONNECTED) {
            Serial.println("WiFi Connect Failed! Rebooting...");
            delay(1000);
            // ESP.restart();
        }
    #endif
}

void dartClient::handleAP()
{
    
}