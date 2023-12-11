#ifndef fnc_h
#define fnc_h

#include <Arduino.h>


/*
 *
 *      Global functions
 *
 */

bool isIp(String str);
void initServer();
bool captivePortal(AsyncWebServerRequest *request);
void serveIndex(AsyncWebServerRequest *request);

void printDirectory(File dir, int numTabs);
#endif
