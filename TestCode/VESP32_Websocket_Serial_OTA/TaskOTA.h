#ifndef TASK_OTA_H
#define TASK_OTA_H

#include <ESPAsyncWebServer.h>
#include <Update.h>

namespace TaskOTA {
    // Thêm inline để có thể include nhiều nơi mà không bị lỗi multiple definition
    inline bool isUpdating = false;
    
    inline const char* adminUser = "admin";
    inline const char* adminPass = "123456";

    // HTML UI (dùng inline cho hằng số chuỗi)
    inline const char upload_html[] PROGMEM = R"rawliteral(
        <!DOCTYPE html><html><head><title>OTA Update</title>
        <meta name="viewport" content="width=device-width, initial-scale=1">
        <style>
            body{font-family:sans-serif; text-align:center; padding:50px; background:#f4f4f4;}
            .box{background:white; padding:30px; border-radius:10px; display:inline-block; box-shadow: 0 4px 6px rgba(0,0,0,0.1);}
            .warning{color:#e74c3c; font-weight:bold;}
        </style>
        </head><body>
            <div class="box">
                <h2>Cập nhật hệ thống</h2>
                <p class="warning">⚠️ WebSocket sẽ bị ngắt để đảm bảo an toàn.</p>
                <form method='POST' action='/update' enctype='multipart/form-data'>
                    <input type='file' name='update' accept='.bin'><br><br>
                    <input type='submit' value='Bắt đầu nạp Firmware' style='padding:10px 20px; cursor:pointer;'>
                </form>
            </div>
        </body></html>)rawliteral";

    // Thêm inline cho hàm init
    inline void init(AsyncWebServer* server, AsyncWebSocket* ws) {
        
        server->on("/ota", HTTP_GET, [](AsyncWebServerRequest *request){
            if(!request->authenticate(adminUser, adminPass)) {
                return request->requestAuthentication();
            }
            request->send_P(200, "text/html", upload_html);
        });

        server->on("/update", HTTP_POST, [](AsyncWebServerRequest *request){
            if(!request->authenticate(adminUser, adminPass)) {
                return request->requestAuthentication();
            }
            bool success = !Update.hasError();
            AsyncWebServerResponse *response = request->beginResponse(200, "text/plain", success ? "OK! Rebooting..." : "FAIL!");
            response->addHeader("Connection", "close");
            request->send(response);
            delay(1000); 
            if (success) ESP.restart();
        }, [ws](AsyncWebServerRequest *request, String filename, size_t index, uint8_t *data, size_t len, bool final){
            
            if(!request->authenticate(adminUser, adminPass)) return;

            if (!index) {
                isUpdating = true;
                ws->textAll("OTA_PROCESS_STARTED");
                ws->closeAll(); 
                Serial.println("OTA Update Started...");
                if (!Update.begin(UPDATE_SIZE_UNKNOWN)) Update.printError(Serial);
            }

            if (Update.write(data, len) != len) Update.printError(Serial);

            if (final) {
                if (Update.end(true)) {
                    Serial.printf("Update hoàn tất: %u bytes\n", index + len);
                } else {
                    Update.printError(Serial);
                    isUpdating = false;
                }
            }
        });
    }
}

#endif