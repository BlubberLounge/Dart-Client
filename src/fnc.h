#ifndef fnc_h
#define fnc_h

#include <Arduino.h>


/*
 *
 *      Global functions
 *
 */

int getPixelIndexByXY(uint8_t x, uint8_t y);
void setPixel(int index);
void setPixelXY(uint8_t x, uint8_t y, uint32_t clr);
void setPixelBri(uint8_t x, uint8_t y, uint8_t bri);


bool isIp(String str);
void initServer();
bool captivePortal(AsyncWebServerRequest *request);
void serveIndex(AsyncWebServerRequest *request);
void onEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type, void *arg, uint8_t *data, size_t len);
void sendDataWs(AsyncWebSocketClient *client);
void cleanupWs();

void printDirectory(File dir, int numTabs);
#endif
