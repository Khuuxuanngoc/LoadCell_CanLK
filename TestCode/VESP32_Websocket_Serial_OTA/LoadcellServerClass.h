#ifndef LOADCELL_SERVER_CLASS_H
#define LOADCELL_SERVER_CLASS_H

#if defined(ESP8266)
  #include <ESP8266WiFi.h>
  #include <ESPAsyncTCP.h>
#elif defined(ESP32)
  #include <WiFi.h>
  #include <AsyncTCP.h>
#endif

#include <ESPAsyncWebServer.h>
#include "TaskOTA.h"

// Callback type để báo về chương trình chính khi người dùng nhấn nút trên Web
typedef void (*ButtonCallback)(String command);

class LoadcellServer {
private:
    AsyncWebServer _server;
    AsyncWebSocket _ws;
    ButtonCallback _callback;

    static void onEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, 
                        AwsEventType type, void *arg, uint8_t *data, size_t len);
    
    // Lưu trữ HTML giao diện (để trong PROGMEM để tiết kiệm RAM)
    static const char INDEX_HTML[] PROGMEM;

public:
    LoadcellServer(uint16_t port = 80);
    
    void begin(ButtonCallback cb);
    void updateData(float gt1, long gt2);
    void cleanup();
};

#endif