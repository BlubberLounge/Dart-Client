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
    
    Serial.println();
    Serial.print(F("esp8266 "));
    Serial.println(ESP.getCoreVersion());
    wifi_set_sleep_type(NONE_SLEEP_T);
  
    initConnection();

    // initilaize all web server routes
    initServer();

    escapedMac = WiFi.macAddress();
    escapedMac.replace(":", "");
    escapedMac.toLowerCase();

    // Set up mDNS responder:
    if (strlen(cmDNS) > 0) {
        MDNS.end();
        MDNS.begin(cmDNS);

        MDNS.addService("http", "tcp", 80);
        MDNS.addService("dart-client", "tcp", 80);
        MDNS.addServiceTxt("dart-client", "tcp", "mac", escapedMac.c_str());
    }
}

void dartClient::loop()
{
    handleConnection();

    if(ap_active) dnsServer.processNextRequest();

    MDNS.update();
}

void dartClient::initConnection()
{
    // WiFi.config(IPAddress((uint32_t)0), IPAddress((uint32_t)0), IPAddress((uint32_t)0));
    WiFi.hostname(WIFI_HOSTNAME);
    WiFi.softAPdisconnect(true);
    WiFi.mode(WIFI_STA);
    WiFi.begin(WIFI_SSID, WIFI_PWD);
    if (WiFi.waitForConnectResult(10000) != WL_CONNECTED) {
        Serial.println("WiFi Connect Failed! Starting in Offline mode...");

        WiFi.disconnect(true);
        WiFi.softAPConfig(IPAddress(4, 3, 2, 1), IPAddress(4, 3, 2, 1), IPAddress(255, 255, 255, 0));

        Serial.print("Starting AP...");
        Serial.println(WiFi.softAP("Test", "12345678") ? "Ready" : "Failed!");

        dnsServer.setErrorReplyCode(DNSReplyCode::NoError);
        dnsServer.start(53, "*", WiFi.softAPIP());
        ap_active = true;
    } 
    
    Serial.print("Device IP: ");
    if(ap_active) {
        Serial.println(WiFi.softAPIP());
    } else {
        Serial.println(WiFi.localIP());
    }
}

void dartClient::handleConnection()
{
    static byte stacO = 0;
    if(ap_active) {
        byte stac = 0;
        stac = wifi_softap_get_station_num();
        if (stac != stacO) {
            stacO = stac;
            Serial.print(F("Connected AP clients: "));
            Serial.println(stac);
            // if (!WLED_CONNECTED && WLED_WIFI_CONFIGURED) {        // trying to connect, but not connected
            //     if (stac)
            //     WiFi.disconnect();        // disable search so that AP can work
            //     else
            //     initConnection();         // restart search
            // }
        }
    }


}