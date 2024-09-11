#define BLYNK_TEMPLATE_ID "Copy_BLYNK_TEMPLATE_ID_From_BlynkCloud"
#define BLYNK_TEMPLATE_NAME "Copy_BLYNK_TEMPLATE_NAME_From_BlynkCloud"
#define BLYNK_AUTH_TOKEN "Copy_BLYNK_AUTH_TOKEN_From_BlynkCloud"

// Thêm bộ thư viện
// Add the library.
#include <MKL_LiquidCrystal_I2C.h>
#include "BlynkGate.h"

// Khởi tạo LCD
// LCD config
MKL_LiquidCrystal_I2C lcd(0x27, 16, 2);

// Khai báo module.
// Declare module.
char auth[] = BLYNK_AUTH_TOKEN;
char ssid[] = "MakerLab.vn";  // Key in your wifi name (Bandwidth 2.4Ghz). You can check with your smart phone for your wifi name
char pass[] = "";             // Key in your wifi password.

void setup() {
  // put your setup code here, to run once:
  // Khởi động LCD
  // LCD start
  lcd.init();
  lcd.backlight();

  // Khởi động kết nối Serial UART ở tốc độ 115200 để truyền dữ liệu lên máy tính.
  // Start the Serial UART connection at 115200 to transfer data to the computer.
  Serial.begin(115200);
  Serial.println("Start");

  // Khởi tạo module
  // init module
  Blynk.begin(auth, ssid, pass);
}

void loop() {
  // put your main code here, to run repeatedly:
  Blynk.run();
  // DO NOT using delay
}

BLYNK_WRITE_DEFAULT() {
  // double myDouble = param.asFloat();
  // String myString = param.asString();
  int myInt = param.asInt();

  
  // Hiển thị giá trị của module lên máy tính.
  // Show the module value on Arduno Serial Monitor
  Serial.print("input V");
  Serial.print(request.pin);
  Serial.print(": ");
  Serial.println(myInt);

  //Gửi giá trị module lên LCD
  //Show the module value on LCD
  lcd.setCursor(0,0);
  lcd.print("input V");
  lcd.print(request.pin);
  lcd.setCursor(0,1);
  lcd.print("value:");
  lcd.print(myInt);
  lcd.print("     ");

}
