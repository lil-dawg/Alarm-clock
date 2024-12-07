#ifndef menu_internals
#define menu_internals

//#include "rotary_encoder.cpp"
#include "custom_char.h"

uint32_t loopStartMs;
boolean updateAllItems;
boolean updateItemValue;
uint8_t itemCnt;
uint8_t pntrPos;
uint8_t dispOffset;
uint8_t root_pntrPos = 1;
uint8_t root_dispOffset = 0;
uint8_t flashCntr;
boolean flashIsOn;
void initMenuPage(String title, uint8_t itemCount);
void captureUserInput();
void adjustBoolean(boolean *v);
void adjustUint8_t(uint8_t *v, uint8_t min, uint8_t max);
void doPointerNavigation();
bool isFlashChanged();
void pacingWait();
bool menuItemPrintable(uint8_t xPos, uint8_t yPos);

void initMenuPage(LiquidCrystal_I2C lcd, String title, uint8_t itemCount){
  lcd.clear();
  lcd.setCursor(0,0);

  uint8_t fillCnt = (DISP_CHAR_WIDTH - title.length()) / 2;
  if (fillCnt > 0){for(uint8_t i = 0; i < fillCnt; i++){lcd.print(F("\04"));}}
  lcd.print(title);
  if ((title.length() % 2) == 1){fillCnt++;}
  if (fillCnt > 0){for(uint8_t i = 0; i < fillCnt; i++){lcd.print(F("\05"));}}

  btnUp.ClearWasDown();
  btnDown.ClearWasDown();
  btnOk.ClearWasDown();
  btnBack.ClearWasDown();
  btnPlus.ClearWasDown();
  btnMinus.ClearWasDown();
  
  itemCnt = itemCount;
  pntrPos = 1;
  dispOffset = 0;
  flashCntr = 0;
  flashIsOn = false;
  updateAllItems = true;
  loopStartMs = millis();
}
void captureUserInput(){
  btnUp.CaptureDownState();
  btnDown.CaptureDownState();
  btnOk.CaptureDownState();
  btnBack.CaptureDownState();
  btnPlus.CaptureDownState();
  btnMinus.CaptureDownState();
}
void adjustBoolean(boolean *v){
  if(btnPlus.PressRealeased() || btnMinus.PressRealeased()){*v = !*v; updateAllItems = true;}
}
void adjustUint8_t(uint8_t *v, uint8_t min, uint8_t max){
  if(btnPlus.RepeatCnt == 0 && btnMinus.Repeated()){if (*v > min){*v = *v - 1; updateItemValue = true;}}

  if(btnMinus.RepeatCnt == 0 && btnPlus.Repeated()){if (*v < max){*v = *v + 1; updateItemValue = true;}}
}

void doPointerNavigation(){
  // if encoder rotate cw move up
  if(CWFlag && pntrPos > 1){
    CWFlag = false;
    flashIsOn = false; flashCntr = 0; printPointer();

    if(pntrPos - dispOffset == 1){updateAllItems = true; dispOffset--;}
    pntrPos--;
  }
  // if encoder rotate ccw move down
  else if(CCWFlag && pntrPos < itemCnt){
    CCWFlag = false;
    flashIsOn = false; flashCntr = 0; printPointer();

    if(pntrPos - dispOffset == DISP_ITEM_ROWS){updateAllItems = true; dispOffset++;}
    pntrPos++;
  }
}

bool isFlashChanged(){
  if(flashCntr == 0){
    flashIsOn = !flashIsOn;
    flashCntr = FLASH_RST_CNT;
    return true;
  }
  else{flashCntr--; return false;}
}
void pacingWait(){
  while(millis() - loopStartMs < PACING_MS){delay(1);}

  loopStartMs = millis();
}
bool menuItemPrintable(uint8_t xPos, uint8_t yPos){
  if(!(updateAllItems || (updateItemValue && pntrPos == yPos))){return false;}

  uint8_t yMaxOffset = 0;

  if(yPos > DISP_ITEM_ROWS) {yMaxOffset = yPos - DISP_ITEM_ROWS;}

  if(dispOffset <= (yPos-1) && dispOffset >= yMaxOffset) {lcd.setCursor(xPos, yPos - dispOffset); return true;}

  return false;
}

#endif