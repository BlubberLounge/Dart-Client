#include "dartClient.h"

#include "html_index.h"
#include "html_other.h"

bool isIp(String str)
{
  for (size_t i = 0; i < str.length(); i++) {
    int c = str.charAt(i);
    if (c != '.' && (c < '0' || c > '9')) return false;
  }

  return true;
}

static String getContentType(AsyncWebServerRequest* request, String filename)
{
  if(request->hasArg("download")) return "application/octet-stream";
  else if(filename.endsWith(".htm")) return "text/html";
  else if(filename.endsWith(".html")) return "text/html";
  else if(filename.endsWith(".css")) return "text/css";
  else if(filename.endsWith(".js")) return "application/javascript";
  else if(filename.endsWith(".json")) return "application/json";
  else if(filename.endsWith(".png")) return "image/png";
  else if(filename.endsWith(".gif")) return "image/gif";
  else if(filename.endsWith(".jpg")) return "image/jpeg";
  else if(filename.endsWith(".ico")) return "image/x-icon";
  return "text/plain";
}

bool handleFileRead(AsyncWebServerRequest* request, String path)
{
  Serial.println("WS FileRead: " + path);
  if(path.endsWith("/")) path += "index.htm";
  if(path.indexOf("sec") > -1) return false;
  String contentType = getContentType(request, path);
  if(LittleFS.exists(path)) {
    request->send(LittleFS, path, contentType);
    return true;
  }
  return false;
}

void initServer()
{
    // index
    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request)
    {
        // if (captivePortal(request)) return;
        serveIndex(request);
    });

    // health / availability check route
    server.on("/ping", HTTP_GET, [](AsyncWebServerRequest *request)
    {
        request->send(200, "text/plain", "pong");
    });

    server.on("/uptime", HTTP_GET, [](AsyncWebServerRequest *request)
    {
        request->send(200, "text/plain", (String)millis());
    });

    server.on("/freeheap", HTTP_GET, [](AsyncWebServerRequest *request)
    {
        request->send(200, "text/plain", (String)ESP.getFreeHeap());
    });

    server.on("/favicon.ico", HTTP_GET, [](AsyncWebServerRequest *request)
    {
        if(!handleFileRead(request, "/favicon.ico")) request->send_P(200, "image/x-icon", favicon, 156);
    });

    server.on("/normalize.css", HTTP_GET, [](AsyncWebServerRequest *request)
    {
        AsyncWebServerResponse *response = request->beginResponse_P(200, "text/css", HTML_normalizeCSS, HTML_normalizeCSS_length);
        response->addHeader("Content-Encoding","gzip");
        request->send(response);
    });

    server.on("/style.css", HTTP_GET, [](AsyncWebServerRequest *request)
    {
        AsyncWebServerResponse *response = request->beginResponse_P(200, "text/css", HTML_styleCSS, HTML_styleCSS_length);
        response->addHeader("Content-Encoding","gzip");
        request->send(response);
    });

    server.on("^\\api\\/v1\\/dart\\/game\\/[({]?[a-fA-F0-9]{8}[-]?([a-fA-F0-9]{4}[-]?){3}[a-fA-F0-9]{12}[})]?$", HTTP_GET, [] (AsyncWebServerRequest *request)
    {
        String sensorId = request->pathArg(0);
        request->send(200, "text/plain", sensorId);
    });
    
    server.onNotFound( [](AsyncWebServerRequest *request)
    {
        if (request->method() == HTTP_OPTIONS) {
            AsyncWebServerResponse *response = request->beginResponse(200);
            response->addHeader(F("Access-Control-Max-Age"), F("7200"));
            request->send(response);
            return;
        } else {
            request->send(404, "text/plain", "Route not found!");
        }
    });

    // Start server
    server.begin();
}

bool captivePortal(AsyncWebServerRequest *request)
{
  if (ON_STA_FILTER(request)) return false; //only serve captive in AP mode
  if (!request->hasHeader("Host")) return false;
  String hostH;
  hostH = request->getHeader("Host")->value();

  if (!isIp(hostH)) {
    AsyncWebServerResponse *response = request->beginResponse(302);
    response->addHeader(F("Location"), F("http://1.3.3.7"));
    request->send(response);
    return true;
  }

  return false;
}

void serveIndex(AsyncWebServerRequest *request)
{
    AsyncWebServerResponse *response;
    response = request->beginResponse_P(200, "text/html", HTML_index, HTML_index_L);
    response->addHeader("Content-Encoding","gzip");
    request->send(response);
}