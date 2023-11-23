#ifndef fnc_h
#define fnc_h

#include <Arduino.h>

/*
 *
 *      Global functions
 * 
 */

void initServer();
void serveIndex(AsyncWebServerRequest *request);

#endif