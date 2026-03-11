#include "LoadcellServerClass.h"

LoadcellServer myServer(80);

float kxnFloatValue = 1000;

// Hàm này sẽ tự động chạy khi ai đó nhấn nút trên Web
void handleWebButtons(String command) {
    Serial.println("Lệnh từ Web: " + command);
    
    if (command == "TARE") {
        // scale.tare();
        kxnFloatValue = 5000;
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
    }
    // ... các lệnh khác tương tự
}

void setup() {
    Serial.begin(115200);
    WiFi.softAP("Can-Dien-Tu", "12345678"); // Tạo WiFi hoặc kết nối WiFi

    myServer.begin(handleWebButtons);
}

void loop() {
    myServer.cleanup(); // Quan trọng để giải phóng RAM của WebSocket

    // Giả lập đọc dữ liệu từ Loadcell
    float value1 = random(100, 500) / 10.0;
    long value2 = random(10000, 20000);

    // Cập nhật lên tất cả các điện thoại đang truy cập
    static unsigned long lastUpdate = 0;
    if (millis() - lastUpdate > 200) {
        myServer.updateData(value1, kxnFloatValue);
        lastUpdate = millis();
    }
}