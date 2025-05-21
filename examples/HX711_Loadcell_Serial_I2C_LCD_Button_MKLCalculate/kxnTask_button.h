#ifndef kxnTask_button__H
#define kxnTask_button__H
#include "kxnTask.h"
#include "kxnTask_LoadCell_Kalman.h"
#include "OneButton.h"

#define BUTTON_PIN 11 /*A1*/
#define BUTTON_PIN_SET_DEFAULT_VALUE 12

OneButton OneButton_kxnTask_button (BUTTON_PIN, true); // Pin , active LOW
OneButton OneButton_kxnTask_button_setdefaultValue (BUTTON_PIN_SET_DEFAULT_VALUE, true); // Pin , active LOW

#include <PciManager.h>
#include <PciListenerImp.h>


void onPinChange(byte changeKind);
void onPinChangeSetDefaultValue(byte changeKind);

PciListenerImp listener(BUTTON_PIN, onPinChange);
PciListenerImp listener2(BUTTON_PIN_SET_DEFAULT_VALUE, onPinChangeSetDefaultValue);

void onPinChange(byte changeKind) {
  Serial.print("pci1 : ");
  Serial.println(changeKind);
  OneButton_kxnTask_button.tick();
}

void onPinChangeSetDefaultValue(byte changeKind) {
  Serial.print("pci2 : ");
  Serial.println(changeKind);
  OneButton_kxnTask_button_setdefaultValue.tick();
}

#define EN_kxnTask_button_DB

#ifndef DEBUG_PRINTER
  #define DEBUG_PRINTER                                                          \
    Serial /**< Define where debug output will be printed.                       \
            */
#endif

#ifdef EN_kxnTask_button_DB
  #define kxnTask_button_DB(...)    { DEBUG_PRINTER.print(__VA_ARGS__); }

#else
  #define kxnTask_button_DB(...)    {}
#endif

CREATE_TASK(kxnTask_button)
kxnTask_LoadCell *myLoadCell;
OneButton *myButton;
OneButton *myButtonSetDefaultValue;
// String *myStringData;

void setup(kxnTask_LoadCell *paLoadCell, int paButtonPin){
  setupButton(paButtonPin);
  kxnTaskManager.add(this);
  this->myLoadCell = paLoadCell;
  Serial.println("Button setup done!");
  setState(0);
  kDelay(10);
}

void setup(kxnTask_LoadCell *paLoadCell, int paButtonPin, int paButtonPin_SetDefaultValue){
  setupButton(paButtonPin);
  setupButtonSetDefaultValue(paButtonPin_SetDefaultValue);
  kxnTaskManager.add(this);
  this->myLoadCell = paLoadCell;
  Serial.println("Button setup 2 done!");
  setState(0);
  kDelay(10);
}

void loop(){
  myButton->tick();
  myButtonSetDefaultValue->tick();
  // Serial.println("Button Loop!");
}

void runFast(){
  // myButton->tick();
  // myButtonSetDefaultValue->tick();
  // Serial.println("Button run fast!");
}

void setupButton(int paButtonPin){
  PciManager.registerListener(paButtonPin, &listener);

  // if(myButton != NULL){
  //   delete myButton;
  // }

  // myButton = new OneButton(paButtonPin, true); // Pin , active LOW
  myButton = &OneButton_kxnTask_button;
  // myButton->setClickTimeout(500);
  // myButton->setPressTicks(1000);
  // myButton->setLongPressStop(0);
  // myButton->setNumberClicks(1);
  myButton->setClickTicks(300);
  myButton->attachClick([](void *object) {
    kxnTask_button * self = (kxnTask_button *)object;
    long tempValue = (self->myLoadCell->getGram() + 0.4);
    self->myLoadCell->setRealValue(tempValue);
    kxnTask_button_DB(F("Button Click Round up\n"));
    // myLoadCell->tare();
  }, this);

  myButton->attachDoubleClick([](void *object) {
    kxnTask_button * self = (kxnTask_button *)object;
    self->myLoadCell->setRealValue(5);
    kxnTask_button_DB(F("Button Double Click Set SL 5\n"));
  }, this);

  myButton->attachLongPressStart([](void *object) {
    kxnTask_button * self = (kxnTask_button *)object;
    self->myLoadCell->tare();
    kxnTask_button_DB(F("Button Long Press Start Tare\n"));
  }, this);
  // myButton->attachDoubleClick(buttonDoubleClick);
  // myButton->attachLongPressStart(buttonLongPressStart);
  // myButton->attachLongPressStop(buttonLongPressStop);
}

void setupButtonSetDefaultValue(int paButtonPin_SetDefaultValue){
  PciManager.registerListener(paButtonPin_SetDefaultValue, &listener2);

  myButtonSetDefaultValue = &OneButton_kxnTask_button_setdefaultValue;
  myButtonSetDefaultValue->setClickTicks(300);
  myButtonSetDefaultValue->attachClick([](void *object) {
    kxnTask_button * self = (kxnTask_button *)object;
    self->myLoadCell->setRealValue(5);
    kxnTask_button_DB(F("Button Set Default Value\n"));
  }, this);
}

END

#endif