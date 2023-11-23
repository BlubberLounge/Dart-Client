#include "dartClient.h"

#include "html_index.h"


void initServer()
{
    // index
    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request)
    {
        serveIndex(request);
    });

    // health / availability check route
    server.on("/ping", HTTP_GET, [](AsyncWebServerRequest *request)
    {
         request->send(200, "text/plain", "pong");
    });
    
    server.onNotFound( [](AsyncWebServerRequest *request)
    {
        request->send(404, "text/plain", "Route not found!");
    });

    // Start server
    server.begin();
}

void serveIndex(AsyncWebServerRequest *request)
{
    AsyncWebServerResponse *response;
    response = request->beginResponse_P(200, "text/html", HTML_index, HTML_index_L);
    response->addHeader("Content-Encoding","gzip");
    request->send(response);
}