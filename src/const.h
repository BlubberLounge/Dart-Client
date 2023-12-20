#ifndef const_h
#define const_h

/*
 *
 *
 */


#include <FS.h>
#include <LittleFS.h>
#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <ESPAsyncWebServer.h>
#include <DNSServer.h>
#include <SD.h>
#include <SPI.h>
#include <AsyncJson.h>
#include <ArduinoJson.h>
#include <FastLED.h>

#define DART_HOST "http://192.168.178.100:81/"
#define DART_API_URL DART_HOST + "api/v1/"

#define MDNS_NAME   "x"

#define MAX_DARTPLAYER 4
#define DARTPLAYER_CODE_SIZE 10


#ifdef USE_OTA
    #include <ArduinoOTA.h>
#endif

#endif
