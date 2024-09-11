/**
 * History:
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
#include "kxnTask_LoadCell.h"
#include "kxnTask_I2CLCD.h"

kxnTask_LoadCell kxnTask_LoadCell1;
kxnTask_I2CLCD kxnTask_I2CLCD1;

void setup() {
  Serial.begin(115200);

  // wait for serial port to connect. Needed for native USB port only
  while (!Serial) {
    delay(10);
  }

  Serial.println("Adafruit HX711 Test!");
  kxnTask_LoadCell1.setup();
  kxnTask_I2CLCD1.setup();
} 

void loop() {
  kxnTaskManager.run(millis());

  kxnTask_I2CLCD1.print(String(kxnTask_LoadCell1.getGram()));
}
