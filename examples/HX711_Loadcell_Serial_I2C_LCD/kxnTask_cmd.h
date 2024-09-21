#ifndef kxnTask_cmd__H
#define kxnTask_cmd__H
#include "kxnTask.h"
#include "kxnTask_LoadCell_Kalman.h"

#define EN_kxnTask_cmd_DB

#ifndef DEBUG_PRINTER
  #define DEBUG_PRINTER                                                          \
    Serial /**< Define where debug output will be printed.                       \
            */
#endif

#ifdef EN_kxnTask_cmd_DB
  #define kxnTask_cmd_DB(...)    { DEBUG_PRINTER.print(__VA_ARGS__); }

#else
  #define kxnTask_cmd_DB(...)    {}
#endif

CREATE_TASK(kxnTask_cmd)
kxnTask_LoadCell *myLoadCell;
// String *myStringData;

void setup(kxnTask_LoadCell *paLoadCell){
  kxnTaskManager.add(this);
  this->myLoadCell = paLoadCell;
}

void loop(){
  
}

void setData(String myStringData){
  if(myStringData.length() > 0){
    String tempString = myStringData;

    kxnTask_cmd_DB(F("Data cmd in: "));
    kxnTask_cmd_DB(tempString);

    kxnTask_cmd_DB(F("Data len: "));
    kxnTask_cmd_DB(String() + myStringData.length() + "\n");

    if(tempString.indexOf("tare") > -1){
      myLoadCell->tare();
    }else if(tempString.indexOf("gram") > -1){
      float tempFloat = tempString.toFloat();
      myLoadCell->setRealValue(tempFloat);
      kxnTask_cmd_DB(String("cmd to Float ") + tempFloat + "\n");
    }

  };
}

END

#endif