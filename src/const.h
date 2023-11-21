/*
 *
 * 
 */

#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <ESP8266WebServer.h>

#include "server.cpp"

ESP8266WebServer server(80);

#ifdef USE_OTA
    #include <ArduinoOTA.h>
#endif