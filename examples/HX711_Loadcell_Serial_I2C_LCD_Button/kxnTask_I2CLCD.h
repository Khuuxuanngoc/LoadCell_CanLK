#pragma once

#include "kxnTask.h"
#include <MKL_LiquidCrystal_I2C.h>

DEFINE_TASK_STATE(kxnTask_I2CLCD){
  kxnTask_I2CLCD_ON,
  kxnTask_I2CLCD_OFF,
};

CREATE_TASK(kxnTask_I2CLCD)
String data2;
MKL_LiquidCrystal_I2C *lcd;  //

void setup() {

  if (!lcd) lcd = new MKL_LiquidCrystal_I2C(0x27, 16, 2);

  this->data2 = "";
  lcd->init();
  lcd->backlight();
  start();
}

void loop() {
  switch (getState()) {
    case kxnTask_I2CLCD_ON:
      lcd->setCursor(1, 0);
      lcd->print(this->data2);
      lcd->print("     ");
      kDelay(200);
      break;

    default:
      break;
  }
}

void start() {
  kxnTaskManager.add(this);
  setState(kxnTask_I2CLCD_ON);
}

void stop() {
  setStateIdle();
}

void print(String pa_Data) {
  this->data2 = pa_Data;
}

void runFast() {  // this function will be called without delay
  // Serial.println("kxnTask_I2CLCD running");
}

END