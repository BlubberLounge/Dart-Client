#define DEFINE_GLOBAL_VARS //only in one source file
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
    // sanity check delay - allows reprogramming if accidently blowing power w/leds
    delay(2000);

    Serial.begin(115200);

    Serial.println();
    Serial.print(F("esp8266 "));
    Serial.println(ESP.getCoreVersion());
    wifi_set_sleep_type(NONE_SLEEP_T);

    initStorage();
    initConnection();
    initDisplay();

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

void dartClient::initStorage()
{
    if(!SD.begin(15)){
        Serial.println("Card Mount Failed");
        return;
    }

    if(!SD.exists("/data"))
        SD.mkdir("/data");

    DynamicJsonDocument doc(1024);
    doc["sensor"] = "gps";
    doc["time"]   = 1351824120;
    doc["data"][0] = 48.756080;
    doc["data"][1] = 2.302038;

    File f;
    f = SD.open("/data/test.json", "w+");
    serializeJson(doc, f);
    f.close();

    File root;
    root = SD.open("/");
    printDirectory(root, 0);

    SD.remove("/data/test.json");
}

void dartClient::initConnection()
{
    // WiFi.config(IPAddress((uint32_t)0), IPAddress((uint32_t)0), IPAddress((uint32_t)0));
    WiFi.hostname(WIFI_HOSTNAME);
    WiFi.softAPdisconnect(true);
    WiFi.mode(WIFI_STA);
    WiFi.begin(WIFI_SSID, WIFI_PWD);
    if (WiFi.waitForConnectResult(20000) != WL_CONNECTED) {
        Serial.println("WiFi Connect Failed! Starting in Offline mode...");

        WiFi.disconnect(true);
        WiFi.softAPConfig(IPAddress(4, 3, 2, 1), IPAddress(4, 3, 2, 1), IPAddress(255, 255, 255, 0));

        Serial.print("Starting AP...");
        Serial.println(WiFi.softAP("Nothing to see here", "0hB4by4Tr1ppl3") ? "Ready" : "Failed!");

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

void dartClient::initDisplay()
{
    FastLED.setMaxPowerInVoltsAndMilliamps(5, 450);
    FastLED.setBrightness( 8 );
    FastLED.addLeds<WS2812B, 4, GRB>(leds, 256);

    FastLED.clear();

    leds[0] = CRGB::Green;

    display.setPoints(41);

    Serial.println("Display init");
}

void dartClient::handleConnection()
{
    static uint8_t stacO = 0;
    if(ap_active) {
        uint8_t stac = 0;
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
