#include "dartClient.h"

#include "html_index.h"


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
    
    server.onNotFound(serveNotFound);

    // Start server
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

void response(String msg)
{
    server.send(200, "text/html", msg);
}

void responseNotFound(String msg)
{
    server.send(404, "text/html", msg);
}

void serveNotFound()
{
    responseNotFound("Route not found");
}

void serveIndex()
{
    // response
    server.send(200, "text/html", (const char*)html_index);
}