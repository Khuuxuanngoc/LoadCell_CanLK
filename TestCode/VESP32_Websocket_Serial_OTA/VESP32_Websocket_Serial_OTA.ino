/**
    y260313:
        - KQ Test: Vietduino ESP32-S3 N16R8
            - Các chức năng: tare, round, set sample, upload firmware OTA OK (không cần USB)
            - Release release_y260313_VESP32_Websocket_Serial_OTA.ino.bin
    y260312: 
        - Fix GUG:
            - Fix lỗi Font OTA Webserver: OK
        - Add file kxnTaskLoadCell_Kalman.h

    y260311: Test Websocket OK
        - Link Gemini: https://gemini.google.com/share/33469e75ff75
        - Fix BUG:
            - HTML btn SET_10 fail
        - Add TaskOTA.h : đang test
            - Nạp OTA OK.
            - Bug: đang lỗi font
*/
#include "LoadcellServerClass.h"
#include "kxnTask_LoadCell_Kalman.h"
kxnTask_LoadCell kxnTask_LoadCell1;
// Define the pins for the HX711 communication VietESP32-S3
const uint8_t DATA_PIN = 18; //2;  // Can use any pins!
const uint8_t CLOCK_PIN = 17; //3; // Can use any pins!

LoadcellServer myServer(80);

float kxnFloatValue = 1000;

// Hàm này sẽ tự động chạy khi ai đó nhấn nút trên Web
void handleWebButtons(String command) {
    Serial.println("Lệnh từ Web: " + command);
    kxnFloatValue = 1000;

    if (command == "TARE") {
        // scale.tare();
        // kxnFloatValue = 5000;
        kxnTask_LoadCell1.tare();
    } else if (command == "SET_25") {
        // Thực hiện logic calib x25
        kxnFloatValue = 25;
    } else if (command == "SET_1") {
        // Thực hiện logic calib x25
        kxnFloatValue = 1;
    } else if (command == "SET_5") {
        // Thực hiện logic calib x25
        kxnFloatValue = 5;
    } else if (command == "SET_10") {
        // Thực hiện logic calib x25
        kxnFloatValue = 10;
    } else if (command == "SET_20") {
        // Thực hiện logic calib x25
        kxnFloatValue = 20;
    } else if (command == "ROUND") {
        // Thực hiện làm tròn
        kxnTask_LoadCell1.round();
        
    }

    if(kxnFloatValue != 1000)
    {
        kxnTask_LoadCell1.setRealValue(kxnFloatValue);
        kxnFloatValue = 1000;

    }
    // ... các lệnh khác tương tự
}

void setup() {
    Serial.begin(115200);
    WiFi.softAP("Can-Dien-Tu", "12345678"); // Tạo WiFi hoặc kết nối WiFi

    myServer.begin(handleWebButtons);
    kxnTask_LoadCell1.setup(DATA_PIN, CLOCK_PIN);
}

void loop() {
    unsigned long tempMillis = millis();
    if (TaskOTA::isUpdating) return; // Dừng mọi logic cân khi đang nạp code
    kxnTaskManager.run(tempMillis);
    myServer.cleanup(); // Quan trọng để giải phóng RAM của WebSocket

    // Giả lập đọc dữ liệu từ Loadcell
    // float value1 = random(100, 500) / 10.0;
    // long value2 = random(10000, 20000);
    float value1 = kxnTask_LoadCell1.getGram();
    long value2 = kxnTask_LoadCell1.getDrawValue();

    // Cập nhật lên tất cả các điện thoại đang truy cập
    static unsigned long lastUpdate = 0;
    if (tempMillis - lastUpdate > 200) {
        myServer.updateData(value1, kxnFloatValue);
        lastUpdate = tempMillis;
    }
}