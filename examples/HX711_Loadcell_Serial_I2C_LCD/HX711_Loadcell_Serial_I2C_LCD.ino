/**
 * History:
 *  - 21/9/2024:
 *    - Can duoc giay A5, hien thi I2C LCD
 *      - Nhap Serial 115200 NL+CR
 *        - tare de ve 0
 *        - 8.00 gram
 *          - 8.00 la so luong vat the
 *          - gram la command
 *  - 11/9/2024: da doc được Gram, hiển thị LCD I2C
 * 
 *  Load cell 20Kg hien thi LCD
 *  - Wiring:
 *      - Loadcell to HX711
 *        - Red ------ E+
 *        - Black ---- E-
 *        - Green ---- A-
 *        - White ---- A+
 *      - HX711 to VUno
 *        - Vcc ------ 5V
 *        - GND ------ GND
 *        - SCK ------ 3
 *        - DT  ------ 2
 *      - I2C LCD to Uno
 *        - Vcc ------ 5V
 *        - GND ------ GND
 *        - SDA ------ SDA
 *        - SCL ------ SCL
 */
#include "kxnTask_LoadCell_Kalman.h"
#include "kxnTask_I2CLCD.h"
#include "kxnTask_cmd.h"

kxnTask_LoadCell kxnTask_LoadCell1;
kxnTask_I2CLCD kxnTask_I2CLCD1;
kxnTask_cmd kxnTask_cmd1;

void setup() {
  Serial.begin(115200);

  // wait for serial port to connect. Needed for native USB port only
  while (!Serial) {
    delay(10);
  }

  Serial.println("Adafruit HX711 Test!");
  kxnTask_LoadCell1.setup();
  kxnTask_I2CLCD1.setup();
  kxnTask_cmd1.setup(&kxnTask_LoadCell1);
} 

void loop() {
  kxnTaskManager.run(millis());

  kxnTask_I2CLCD1.print(String(kxnTask_LoadCell1.getGram()));

  checkSerialPC(&Serial);
}

void checkSerialPC(Stream* paSerial){
  if(paSerial->available()){
    String tempStr = paSerial->readStringUntil('\n');
    kxnTask_cmd1.setData(tempStr);
  }
}
