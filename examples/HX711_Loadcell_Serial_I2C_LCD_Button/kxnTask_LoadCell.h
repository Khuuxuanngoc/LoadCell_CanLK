#pragma once

#include "kxnTask.h"

#include "Adafruit_HX711.h"


DEFINE_TASK_STATE(kxnTask_LoadCell){
  kxnTask_LoadCell_ON,
  kxnTask_LoadCell_OFF,
};

CREATE_TASK(kxnTask_LoadCell)
// uint8_t pin;
unsigned long timeOn;
// unsigned long timeOff;

// Define the pins for the HX711 communication
// uint8_t DATA_PIN = 2;   // Can use any pins!
// uint8_t CLOCK_PIN = 3;  // Can use any pins!

// Adafruit_HX711 hx711(DATA_PIN, CLOCK_PIN);
Adafruit_HX711 *hx711;
int32_t gramDataA;
void setup() {
  // Initialize the HX711
  if(!hx711) hx711 = new Adafruit_HX711(2,3);

  hx711->begin();

  // read and toss 3 values each
  Serial.println("Tareing....");
  for (uint8_t t = 0; t < 3; t++) {
    hx711->tareA(hx711->readChannelRaw(CHAN_A_GAIN_128));
    hx711->tareA(hx711->readChannelRaw(CHAN_A_GAIN_128));
    // hx711->tareB(hx711->readChannelRaw(CHAN_B_GAIN_32));
    // hx711->tareB(hx711->readChannelRaw(CHAN_B_GAIN_32));
  }
  start();
}

void loop() {
  switch (getState()) {
    case kxnTask_LoadCell_ON:

      // Read from Channel A with Gain 128, can also try CHAN_A_GAIN_64 or CHAN_B_GAIN_32
      // since the read is blocking this will not be more than 10 or 80 SPS (L or H switch)
      int32_t weightA128 = hx711->readChannelBlocking(CHAN_A_GAIN_128);
      gramDataA = (weightA128*95/100) / 100;
      Serial.print("Channel A (Gain 128): ");
      Serial.println(gramDataA);

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
  setState(kxnTask_LoadCell_ON);
}

void stop() {
  setStateIdle();
}

int32_t getGram(){
  return gramDataA;
}

void runFast() {  // this function will be called without delay
  // Serial.println("kxnTask_LoadCell running");
}

END