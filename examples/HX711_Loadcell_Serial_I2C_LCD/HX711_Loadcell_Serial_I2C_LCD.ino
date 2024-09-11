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
