#ifndef fnc_h
#define fnc_h

#include <Arduino.h>

/*
 *
 *      Global functions
 * 
 */

void initServer();
void checkAuth();
void response(String msg);
void responseNotFound(String msg);
void serveNotFound();
void serveIndex();

#endif