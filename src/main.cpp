#include <Arduino.h>
#include <Wire.h>
#include <liquidCrystal_I2C.h>
#include <EEPROM.h>
#include <PressButton.h>
#include "custom_char.h"
#include "rotaryEncoder.h"

#define DISP_ITEM_ROWS 3 
#define DISP_CHAR_WIDTH 20
#define PACING_MS 25
#define FLASH_RST_CNT 30          //used for flashing cursor
#define SETTINGS_CHKVAL 3647

// =========================================================================
//                               DECLARATIONS
// =========================================================================

// MENU STRUCTURE ----------------------------------------------------------
enum pageType {
  MENU_ROOT,
  MENU_SUB1,
  MENU_SUB1_A,
  MENU_SUB1_B,
  MENU_SUB2,
  MENU_SUB3,
  MENU_SUB4,
  MENU_SETTINGS
};
enum pageType currPage = MENU_ROOT;
void page_MenuRoot();
void page_MenuSub1();
void page_MenuSub1_A();
void page_MenuSub1_B();
void page_MenuSub2();
void page_MenuSub3();
void page_MenuSub4();
void page_MenuSettings();

// MENU INTERNALS ----------------------------------------------------------
unsigned int loopStartMs;
bool updateAllItems;
bool updateItemValue;
unsigned char itemCnt;
unsigned char pntrPos;
unsigned char dispOffset;
unsigned char root_pntrPos = 1;
unsigned char root_dispOffset = 0;
unsigned char flashCntr;
bool flashIsOn;
void initMenuPage(String title, unsigned char itemCount);
void captureUserInput();
bool adjustingValue = false;
void adjustBool(bool *v, bool *adjustingValue);
void adjustChar(unsigned char *v, bool *adjustingValue, unsigned char min, unsigned char max);
void doPointerNavigation();
bool isFlashChanged();
void pacingWait();
bool menuItemPrintable(unsigned char xPos, unsigned char yPos);

// PRINT TOOLS -------------------------------------------------------------
void printPointer();
void printOffsetArrows();
void printOnOff(bool val);
void printintAtWidth(unsigned int value, unsigned char width, char c, bool isRight);

// SETTINGS ----------------------------------------------------------------
struct MySettings{
  bool Test1_OnOff = false;
  unsigned char Test2_Num = 60;
  unsigned char Test3_Num = 255;
  unsigned char Test4_Num = 0;
  bool Test5_OnOff = false;
  unsigned char Test6_Num = 197;

  unsigned short settingCheckValue = SETTINGS_CHKVAL;
  
};
MySettings settings;
void sets_SetDefaults();
void sets_Load();
void sets_Save();

// DISPLAY -----------------------------------------------------------------
LiquidCrystal_I2C lcd(0x27, 20, 4);

// =========================================================================
//                                  SETUP
// =========================================================================
void setup() {
  encoder_setup(pinA, pinB, BTN_OK, prevPinA);

  lcd.init();
  createChars(lcd);
  lcd.backlight();
  lcd.clear();

  sets_Load();

  // rotary encoder interrupt
  prevTime = millis();

  Serial.begin(9600);
}

// =========================================================================
//                                MAIN LOOP
// =========================================================================
void loop() {
  switch(currPage){
    case MENU_ROOT: page_MenuRoot();break;
    case MENU_SUB1: page_MenuSub1();break;
    case MENU_SUB1_A: page_MenuSub1_A();break;
    case MENU_SUB1_B: page_MenuSub1_B();break;
    case MENU_SUB2: page_MenuSub2();break;
    case MENU_SUB3: page_MenuSub3();break;
    case MENU_SUB4: page_MenuSub4();break;
    case MENU_SETTINGS: page_MenuSettings();break;
    
  }
}

// =========================================================================
//                                MENU ROOT
// =========================================================================
void page_MenuRoot(){
  initMenuPage(F("Carinas Meny"), 5);

  pntrPos = root_pntrPos; dispOffset = root_dispOffset;

  while(true){
    if(updateAllItems){
      if(menuItemPrintable(1, 1)){lcd.print(F("Tomat        "));}
      if(menuItemPrintable(1, 2)){lcd.print(F("Gurka        "));}
      if(menuItemPrintable(1, 3)){lcd.print(F("Physialis    "));}
      if(menuItemPrintable(1, 4)){lcd.print(F("Sub Menu #4  "));}
      if(menuItemPrintable(1, 5)){lcd.print(F("Installningar"));}

      printOffsetArrows();
    }

    if(isFlashChanged()){printPointer();}

    updateAllItems = false;

    captureUserInput();

    if(btnOk.PressRealeased()){
        root_pntrPos = pntrPos; root_dispOffset = dispOffset;
      switch(pntrPos){
        case 1: currPage = MENU_SUB1; return;
        case 2: currPage = MENU_SUB2; return;
        case 3: currPage = MENU_SUB3; return;
        case 4: currPage = MENU_SUB4; return;
        case 5: currPage = MENU_SETTINGS; return;
      }
    }
    //char encoder = readEncoder(CLK, DT, lastStateCLK);
    //if(encoder){Serial.println(int(encoder));}

    doPointerNavigation();

    pacingWait();
  }
};

// =========================================================================
//                                MENU SUB#1
// =========================================================================
void page_MenuSub1(){
  initMenuPage(F("SUB MENU 1"), 3);

  while(true){
    if(updateAllItems){
      if(menuItemPrintable(1, 1)){lcd.print(F("Sub Menu #1_A"));}
      if(menuItemPrintable(1, 2)){lcd.print(F("Sub Menu #2_B"));}
      if(menuItemPrintable(1, 3)){lcd.print(F("Return       "));}

      printOffsetArrows();
    }

    if(isFlashChanged()){printPointer();}

    updateAllItems = false;

    captureUserInput();

    if(btnOk.PressRealeased()){
      switch(pntrPos){
        case 1: currPage = MENU_SUB1_A; return;
        case 2: currPage = MENU_SUB1_B; return;
        case 3: currPage = MENU_ROOT; return;
      }
    }

    doPointerNavigation();

    pacingWait();
  }
};

// =========================================================================
//                                MENU SUB#1_A
// =========================================================================
void page_MenuSub1_A(){
initMenuPage(F("SUB MENU 1_A"), 1);

  while(true){
    if(updateAllItems){
      if(menuItemPrintable(1, 1)){lcd.print(F("No items here!"));}

      printOffsetArrows();
    }

    if(isFlashChanged()){printPointer();}

    updateAllItems = false;

    captureUserInput();
    
    if(btnOk.PressRealeased()){currPage = MENU_SUB1; return;}

    doPointerNavigation();

    pacingWait();
  }
}

// =========================================================================
//                                MENU SUB#1_B
// =========================================================================
void page_MenuSub1_B(){
  initMenuPage(F("SUB MENU 1_B"), 1);

  while(true){
    if(updateAllItems){
      if(menuItemPrintable(1, 1)){lcd.print(F("No items here!"));}

      printOffsetArrows();
    }

    if(isFlashChanged()){printPointer();}

    updateAllItems = false;

    captureUserInput();
    
    if(btnOk.PressRealeased()){currPage = MENU_SUB1; return;}

    doPointerNavigation();

    pacingWait();
  }
};

// =========================================================================
//                                MENU SUB#2
// =========================================================================
void page_MenuSub2(){
  initMenuPage(F("SUB MENU 2"), 3);

  while(true){
    if(updateAllItems){
      if(menuItemPrintable(1, 1)){lcd.print(F("Something One"));}
      if(menuItemPrintable(1, 2)){lcd.print(F("Something Two"));}
      if(menuItemPrintable(1, 3)){lcd.print(F("Return       "));}

      printOffsetArrows();
    }

    if(isFlashChanged()){printPointer();}

    updateAllItems = false;

    captureUserInput();
    
    if(btnOk.PressRealeased() && pntrPos == 3){currPage = MENU_ROOT; return;}

    doPointerNavigation();

    pacingWait();
  }
}

// =========================================================================
//                                MENU SUB#3
// =========================================================================
void page_MenuSub3(){
  initMenuPage(F("SUB MENU 3"), 3);

  while(true){
    if(updateAllItems){
      if(menuItemPrintable(1, 1)){lcd.print(F("Something One  "));}
      if(menuItemPrintable(1, 2)){lcd.print(F("Something Two  "));}
      if(menuItemPrintable(1, 3)){lcd.print(F("Something Three"));}

      printOffsetArrows();
    }

    if(isFlashChanged()){printPointer();}

    updateAllItems = false;

    captureUserInput();

    doPointerNavigation();

    pacingWait();
  }
};

// =========================================================================
//                                MENU SUB#4
// =========================================================================
void page_MenuSub4(){
  initMenuPage(F("SUB MENU 4"), 4);

  while(true){
    if(updateAllItems){
      if(menuItemPrintable(1, 1)){lcd.print(F("Something One  "));}
      if(menuItemPrintable(1, 2)){lcd.print(F("Something Two  "));}
      if(menuItemPrintable(1, 3)){lcd.print(F("Something Three"));}
      if(menuItemPrintable(1, 4)){lcd.print(F("Something Four "));}

      printOffsetArrows();
    }

    if(isFlashChanged()){printPointer();}

    updateAllItems = false;

    captureUserInput();

    doPointerNavigation();

    pacingWait();
  }
};

// =========================================================================
//                               MENU SETTINGS
// =========================================================================
void page_MenuSettings(){
  initMenuPage(F("SETTINGS"), 6);

  while(true){
    if(updateAllItems){
      if(menuItemPrintable(1, 1)){lcd.print(F("Auto mode  = "));}
      if(menuItemPrintable(1, 2)){lcd.print(F("Open  temp = "));}
      if(menuItemPrintable(1, 3)){lcd.print(F("Close temp = "));}
      if(menuItemPrintable(1, 4)){lcd.print(F("Setting #4 = "));}
      if(menuItemPrintable(1, 5)){lcd.print(F("Setting #5 = "));}
      if(menuItemPrintable(1, 6)){lcd.print(F("Setting #6 = "));}
      printOffsetArrows();
    }

    if(updateAllItems || updateItemValue){
      if(menuItemPrintable(14, 1)){printOnOff(settings.Test1_OnOff);}
      if(menuItemPrintable(14, 2)){printintAtWidth(settings.Test2_Num, 3, ' ', false);}
      if(menuItemPrintable(14, 3)){printintAtWidth(settings.Test3_Num, 3, ' ', false);}
      if(menuItemPrintable(14, 4)){printintAtWidth(settings.Test4_Num, 3, ' ', false);}
      if(menuItemPrintable(14, 5)){printOnOff(settings.Test5_OnOff);}
      if(menuItemPrintable(14, 6)){printintAtWidth(settings.Test6_Num, 3, ' ', false);}
    }

    if(isFlashChanged()){printPointer();}

    updateAllItems = false;
    updateItemValue = false;

    captureUserInput();

    switch(pntrPos){
      case 1: adjustBool(&settings.Test1_OnOff, &adjustingValue); break;
      case 2: adjustChar(&settings.Test2_Num, &adjustingValue, 0, 255); break;
      case 3: adjustChar(&settings.Test3_Num, &adjustingValue, 0, 255); break;
      case 4: adjustChar(&settings.Test4_Num, &adjustingValue, 0, 255); break;
      case 5: adjustBool(&settings.Test5_OnOff, &adjustingValue); break;
      case 6: adjustChar(&settings.Test6_Num, &adjustingValue, 0, 255); break;
    }
    if(!adjustingValue){
      doPointerNavigation();
    }

    if (btnOk.LongPressed()){sets_SetDefaults(); updateAllItems = true;}

    pacingWait();
  }
};

// =========================================================================
//                          TOOLS - MENU INTERNALS
// =========================================================================
void initMenuPage(String title, unsigned char itemCount){
  lcd.clear();
  lcd.setCursor(0,0);

  unsigned char fillCnt = (DISP_CHAR_WIDTH - title.length()) / 2;
  if (fillCnt > 0){for(unsigned char i = 0; i < fillCnt; i++){lcd.print(F("\04"));}}
  lcd.print(title);
  if ((title.length() % 2) == 1){fillCnt++;}
  if (fillCnt > 0){for(unsigned char i = 0; i < fillCnt; i++){lcd.print(F("\05"));}}

  btnOk.ClearWasDown();
  
  itemCnt = itemCount;
  pntrPos = 1;
  dispOffset = 0;
  flashCntr = 0;
  flashIsOn = false;
  updateAllItems = true;
  loopStartMs = millis();
}
void adjustBool(bool *v, bool *adjustingValue){
  if(*adjustingValue){
    if(CWFlag || CCWFlag){*v = !*v; updateAllItems = true; CWFlag = false; CCWFlag = false;}
  }
  if(btnOk.PressRealeased()){*adjustingValue = !*adjustingValue; btnOk.ClearWasDown();}
}
void adjustChar(unsigned char *v, bool *adjustingValue, unsigned char min, unsigned char max){
  if(*adjustingValue){
    if(CWFlag){if (*v > min){*v = *v - 1; updateItemValue = true;} CWFlag = false;}

    if(CCWFlag){if (*v < max){*v = *v + 1; updateItemValue = true;} CCWFlag = false;}
  }
  if(btnOk.PressRealeased()){*adjustingValue = !*adjustingValue; btnOk.ClearWasDown();}
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
bool menuItemPrintable(unsigned char xPos, unsigned char yPos){
  if(!(updateAllItems || (updateItemValue && pntrPos == yPos))){return false;}

  unsigned char yMaxOffset = 0;

  if(yPos > DISP_ITEM_ROWS) {yMaxOffset = yPos - DISP_ITEM_ROWS;}

  if(dispOffset <= (yPos-1) && dispOffset >= yMaxOffset) {lcd.setCursor(xPos, yPos - dispOffset); return true;}

  return false;
}

// =========================================================================
//                             TOOLS - DISPLAY
// =========================================================================
void printPointer(){
  if(itemCnt < 2){return;}
  lcd.setCursor(0, pntrPos - dispOffset);
  if(flashIsOn){lcd.print(F("\06"));}
  else{lcd.print(F(" "));}
}

void printOffsetArrows(){
  lcd.setCursor(DISP_CHAR_WIDTH - 1, 1);
  if(dispOffset > 0){lcd.print(F("\02"));} else{lcd.print(F(" "));}

  lcd.setCursor(DISP_CHAR_WIDTH - 1, DISP_ITEM_ROWS);
  if(itemCnt > DISP_ITEM_ROWS && itemCnt - DISP_ITEM_ROWS > dispOffset){lcd.print(F("\03"));} else{lcd.print(F(" "));}
}

void printOnOff(bool val){
  if (val){lcd.print(F("ON "));}
  else {lcd.print(F("OFF"));}
}

void printChars(unsigned char cnt, char c){
  if (cnt > 0){
    char cc[] = " "; cc[0] = c;

    for (unsigned char i = 1; i <= cnt; i++){lcd.print(cc);}
  }
}

unsigned char getintCharCnt(unsigned int value){
  if(value==0){return 1;}
  unsigned int tensCalc = 10; unsigned char cnt = 1;

  while (tensCalc <= value && cnt < 20){tensCalc *= 10; cnt +=1;}
  return cnt;
}

void printintAtWidth(unsigned int value, unsigned char width, char c, bool isRight){
  unsigned char numChars = getintCharCnt(value);
  if(isRight){printChars(width-numChars, c);}
  lcd.print(value);

  if(!isRight){printChars(width-numChars, c);}
}

// =========================================================================
//                             TOOLS - SETTINGS
// =========================================================================

void sets_SetDefaults(){
  MySettings tempSets;
  memcpy(&settings, &tempSets, sizeof settings);
}

void sets_Load(){
  EEPROM.get(0, settings);

  if(settings.settingCheckValue != SETTINGS_CHKVAL){sets_SetDefaults();}
}

void sets_Save(){
  EEPROM.put(0, settings);
}