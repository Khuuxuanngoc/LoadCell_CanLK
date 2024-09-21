#pragma once

#include "kxnTask.h"

#include "Adafruit_HX711.h"
#include <SimpleKalmanFilter.h>

#define EN_kxnTask_LoadCell_DB

#ifndef DEBUG_PRINTER
  #define DEBUG_PRINTER                                                          \
    Serial /**< Define where debug output will be printed.                       \
            */
#endif

#ifdef EN_kxnTask_LoadCell_DB
  #define kxnTask_LoadCell_DB(...)    { DEBUG_PRINTER.print(__VA_ARGS__); }

#else
  #define kxnTask_LoadCell_DB(...)    {}
#endif


DEFINE_TASK_STATE(kxnTask_LoadCell){
  kxnTask_LoadCell_ON,
  kxnTask_LoadCell_Tare,
  kxnTask_LoadCell_Tare_Count
};

CREATE_TASK(kxnTask_LoadCell)
unsigned long timeOn;
float scaleValue;
uint8_t numTimeCalib, numTimeCalibCount;
Adafruit_HX711 *hx711;

SimpleKalmanFilter *simpleKalmanFilter;

int32_t gramDataA;
int32_t drawDataA;
void setup(uint8_t paPinSCK, uint8_t paPinDT) {
  // Initialize the HX711
  if (this->hx711) delete this->hx711;
  
  this->hx711 = new Adafruit_HX711(paPinSCK, paPinDT);

  if (!this->simpleKalmanFilter) this->simpleKalmanFilter = new SimpleKalmanFilter(200, 200, 0.2);

  hx711->begin();

  // read and toss 3 values each
  kxnTask_LoadCell_DB("Tareing....\n");
  // hx711->readChannelRaw(CHAN_A_GAIN_128);
  // hx711->readChannelRaw(CHAN_A_GAIN_128);
  // for (uint8_t t = 0; t < 10; t++) {
  //   float tempValue = simpleKalmanFilter->updateEstimate(hx711->readChannelRaw(CHAN_A_GAIN_128));


  //   hx711->tareA(tempValue);

  //   // hx711->tareA(hx711->readChannelRaw(CHAN_A_GAIN_128));
  //   // hx711->tareA(hx711->readChannelRaw(CHAN_A_GAIN_128));
  // }
  this->numTimeCalib = 10; // Init calib
  this->scaleValue = 1;
  start();
}

void setup(){
  this->setup(2,3);
}

void loop() {
  switch (getState()) {
    case kxnTask_LoadCell_Tare:
      hx711->readChannelRaw(CHAN_A_GAIN_128);
      hx711->readChannelRaw(CHAN_A_GAIN_128);
      this->numTimeCalibCount = 0;
      
      setState(kxnTask_LoadCell_Tare_Count);
      kDelay(100);
      break;

      
    case kxnTask_LoadCell_Tare_Count:
      if((this->numTimeCalib - this->numTimeCalibCount) > 0){
        this->numTimeCalibCount++;
        float tempValue = simpleKalmanFilter->updateEstimate(hx711->readChannelRaw(CHAN_A_GAIN_128));
        hx711->tareA(tempValue);
        kDelay(1);
      }else{
        setState(kxnTask_LoadCell_ON);
        kDelay(0);
      }

      // for (uint8_t t = 0; t < 10; t++) {
      //   float tempValue = simpleKalmanFilter->updateEstimate(hx711->readChannelRaw(CHAN_A_GAIN_128));
      //   hx711->tareA(tempValue);
      // }
      // setState(kxnTask_LoadCell_ON);
      break;

      
    case kxnTask_LoadCell_ON:

      // Read from Channel A with Gain 128, can also try CHAN_A_GAIN_64 or CHAN_B_GAIN_32
      // since the read is blocking this will not be more than 10 or 80 SPS (L or H switch)
      drawDataA = hx711->readChannelBlocking(CHAN_A_GAIN_128) + 0.5;
      float tempDrawDataA_Filter = simpleKalmanFilter->updateEstimate(drawDataA);
      gramDataA = tempDrawDataA_Filter;  // / 100;
      kxnTask_LoadCell_DB("Channel A: ");
      kxnTask_LoadCell_DB(String() + gramDataA + " gr\t");
      kxnTask_LoadCell_DB(String() + drawDataA + " unit\n");

      // // Read from Channel A with Gain 128, can also try CHAN_A_GAIN_64 or CHAN_B_GAIN_32
      // int32_t weightB32 = hx711->readChannelBlocking(CHAN_B_GAIN_32);
      // Serial.print("Channel B (Gain 32): ");
      // Serial.println(weightB32);
      kDelay(100);
      break;

    default:
      break;
  }
}

void start() {
  kxnTaskManager.add(this);
  setState(kxnTask_LoadCell_Tare);
}

void stop() {
  setStateIdle();
}

float getGram() {
  return this->gramDataA * this->scaleValue;
}

int32_t getDrawValue() {
  return this->drawDataA;
}

void setRealValue(float pa_realValue){
  this->scaleValue = pa_realValue/this->gramDataA;
  kxnTask_LoadCell_DB(String("Set Scale: ") + this->scaleValue + "\n");
}

void tare(){
  // kxnTaskManager.add(this);
  setState(kxnTask_LoadCell_Tare);
}

void runFast() {  // this function will be called without delay
  // Serial.println("kxnTask_LoadCell running");
}

END