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
void onEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type, void *arg, uint8_t *data, size_t len);
void sendDataWs(AsyncWebSocketClient *client);

void printDirectory(File dir, int numTabs);
#endif
