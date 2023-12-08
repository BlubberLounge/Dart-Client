#ifndef const_h
#define const_h

/*
 *
 * 
 */

#include <LittleFS.h>
#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <ESPAsyncWebServer.h>
#include <DNSServer.h>

#define DART_HOST "http://192.168.178.100:81/"
#define DART_API_URL DART_HOST + "api/v1/"

#define MDNS_NAME   "x"


#ifdef USE_OTA
    #include <ArduinoOTA.h>
#endif

#endif