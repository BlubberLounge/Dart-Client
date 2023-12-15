#include "dartClient.h"

#include "html_other.h"
#include "html_index.h"
#include "html_dart.h"

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
        if (captivePortal(request)) return;
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

    server.on("/isOnline", HTTP_GET, [](AsyncWebServerRequest *request)
    {
        request->send(200, "text/plain", (String)(WIFI_CONNECTED ? "true" : "false"));
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

    server.on("/addPlayer", HTTP_POST, [] (AsyncWebServerRequest *request)
    {
        // for mass adding players
        // for(uint8_t i = 0; i < request->params(); i++) {
            // int index = request->getParam(i)->value().indexOf(":");
            // const String code = request->getParam(i)->value().substring(0, index); // index-1, to strip checksum
            // const String name = request->getParam(i)->value().substring(index+1);

            // dart.addPlayer(code, name);
        // }

        if(!request->hasParam("code", true) || !request->hasParam("name", true)) {
            request->send(200, "application/json", "{\"message\": \"missing parameter\"}");
            return;
        }

        String code = request->getParam("code", true)->value();
        String name = request->getParam("name", true)->value();

        // TODO: verify IEC7064 code
        // server-side validation

        dart.addPlayer(code, name);
        request->send(200, "application/json", "{\"message\": \"ok\"}");
    });

    server.on("/create", HTTP_POST, [] (AsyncWebServerRequest *request)
    {
        dart.setStatus(DartGameStatus::created);

        request->redirect("/game");
    });

    server.on("/state", HTTP_GET, [] (AsyncWebServerRequest *request)
    {
        AsyncJsonResponse *response = new AsyncJsonResponse();
        JsonVariant json = response->getRoot();
        JsonObject state = json.createNestedObject("state");
        state[F("isOnline")] = WIFI_CONNECTED;

        JsonObject game = json.createNestedObject("game");
        dart.serialize(game);

        response->setLength();
        request->send(response);
    });

    // server.on("^dart\\/game\\/[({]?[a-fA-F0-9]{8}[-]?([a-fA-F0-9]{4}[-]?){3}[a-fA-F0-9]{12}[})]?$", HTTP_GET, [] (AsyncWebServerRequest *request)
    // {
    //     String sensorId = request->pathArg(0);
    //     request->send(200, "text/plain", sensorId);
    // });

    server.on("/game", HTTP_GET, [] (AsyncWebServerRequest *request)
    {
        AsyncWebServerResponse *response;
        if(dart.getStatus() == DartGameStatus::created)
            dart.setStatus(DartGameStatus::started);

        if(dart.getStatus() == DartGameStatus::started || dart.getStatus() == DartGameStatus::running) {
            response = request->beginResponse_P(200, "text/html", HTML_dart, HTML_dart_L);
            response->addHeader("Content-Encoding","gzip");
        } else {
            request->send(200, "text/plain", "Game is currently not available. Current game state: " + dart.getStatusString());
            return;
        }

        request->send(response);
    });

    AsyncCallbackJsonWebHandler* handler = new AsyncCallbackJsonWebHandler("/addThrow", [](AsyncWebServerRequest *request, JsonVariant &json)
    {
        if(dart.getStatus() == DartGameStatus::started)
            dart.setStatus(DartGameStatus::running);

        const JsonObject &obj = json.as<JsonObject>();
        uint8_t value = obj["value"];

        DartThrow t;
        t.setValue(value);
        dart.addThrow(t);

        request->send(200, "application/json", "{\"message\": \""+ (String)value +"\"}");

    }, 10240);
    server.addHandler(handler);

    ws.onEvent(onEvent);
    server.addHandler(&ws);

    server.onNotFound([](AsyncWebServerRequest *request)
    {
        // fix for CORS
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

void onEvent(AsyncWebSocket * server, AsyncWebSocketClient * client, AwsEventType type, void * arg, uint8_t *data, size_t len)
{
  if(type == WS_EVT_CONNECT){
    //client connected
    os_printf("ws[%s][%u] connect\n", server->url(), client->id());
    client->printf("Hello Client %u :)", client->id());
    client->ping();
  } else if(type == WS_EVT_DISCONNECT){
    //client disconnected
    os_printf("ws[%s][%u] disconnect: %u\n", server->url(), client->id());
  } else if(type == WS_EVT_ERROR){
    //error was received from the other end
    os_printf("ws[%s][%u] error(%u): %s\n", server->url(), client->id(), *((uint16_t*)arg), (char*)data);
  } else if(type == WS_EVT_PONG){
    //pong message was received (in response to a ping request maybe)
    os_printf("ws[%s][%u] pong[%u]: %s\n", server->url(), client->id(), len, (len)?(char*)data:"");
  } else if(type == WS_EVT_DATA){
    //data packet
    AwsFrameInfo * info = (AwsFrameInfo*)arg;
    if(info->final && info->index == 0 && info->len == len){
      //the whole message is in a single frame and we got all of it's data
      os_printf("ws[%s][%u] %s-message[%llu]: ", server->url(), client->id(), (info->opcode == WS_TEXT)?"text":"binary", info->len);
      if(info->opcode == WS_TEXT){
        data[len] = 0;
        os_printf("%s\n", (char*)data);
      } else {
        for(size_t i=0; i < info->len; i++){
          os_printf("%02x ", data[i]);
        }
        os_printf("\n");
      }
      if(info->opcode == WS_TEXT)
        client->text("I got your text message");
      else
        client->binary("I got your binary message");
    } else {
      //message is comprised of multiple frames or the frame is split into multiple packets
      if(info->index == 0){
        if(info->num == 0)
          os_printf("ws[%s][%u] %s-message start\n", server->url(), client->id(), (info->message_opcode == WS_TEXT)?"text":"binary");
        os_printf("ws[%s][%u] frame[%u] start[%llu]\n", server->url(), client->id(), info->num, info->len);
      }

      os_printf("ws[%s][%u] frame[%u] %s[%llu - %llu]: ", server->url(), client->id(), info->num, (info->message_opcode == WS_TEXT)?"text":"binary", info->index, info->index + len);
      if(info->message_opcode == WS_TEXT){
        data[len] = 0;
        os_printf("%s\n", (char*)data);
      } else {
        for(size_t i=0; i < len; i++){
          os_printf("%02x ", data[i]);
        }
        os_printf("\n");
      }

      if((info->index + len) == info->len){
        os_printf("ws[%s][%u] frame[%u] end[%llu]\n", server->url(), client->id(), info->num, info->len);
        if(info->final){
          os_printf("ws[%s][%u] %s-message end\n", server->url(), client->id(), (info->message_opcode == WS_TEXT)?"text":"binary");
          if(info->message_opcode == WS_TEXT)
            client->text("I got your text message");
          else
            client->binary("I got your binary message");
        }
      }
    }
  }
}

// void sendDataWs(AsyncWebSocketClient * client)
// {
//     DynamicJsonBuffer jsonBuffer;
//     JsonObject& root = jsonBuffer.createObject();
//     root["a"] = "abc";
//     root["b"] = "abcd";
//     root["c"] = "abcde";
//     root["d"] = "abcdef";
//     root["e"] = "abcdefg";
//     size_t len = root.measureLength();
//     AsyncWebSocketMessageBuffer * buffer = ws.makeBuffer(len); //  creates a buffer (len + 1) for you.
//     if (buffer) {
//         root.printTo((char *)buffer->get(), len + 1);
//         if (client) {
//             client->text(buffer);
//         } else {
//             ws.textAll(buffer);
//         }
//     }
// }
