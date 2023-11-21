#include "dartClient.h"

void initServer()
{
    // index
    server.on("/", HTTP_GET, []()
    {
        checkAuth();  
        serveIndex();
    });

    // health / availability check route
    server.on("/ping", HTTP_GET, []()
    {
        server.send(200, "text/plain", "pong");
    });

    server.begin();
}

void checkAuth()
{
    #ifdef USE_WWW_AUTH
        if (!server.authenticate(WWW_USER, WWW_PWD)) {
            return server.requestAuthentication();
        }
    #endif
}

void serveIndex()
{
    // response
    server.send(200, "text/plain", "Login OK");
}