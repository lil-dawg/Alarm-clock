#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#define arrayLen 6

uint8_t customChar[arrayLen][8] = {
{
  0b00110,
  0b01001,
  0b01001,
  0b00110,
  0b00000,
  0b00000,
  0b00000,
  0b00000
},
{
  0b00000,
  0b00100,
  0b00100,
  0b01110,
  0b01110,
  0b11111,
  0b11111,
  0b00000
},
{
  0b00000,
  0b11111,
  0b11111,
  0b01110,
  0b01110,
  0b00100,
  0b00100,
  0b00000
},
{
  0b10000,
  0b01000,
  0b10100,
  0b01010,
  0b10100,
  0b01000,
  0b10000,
  0b00000
},
{
  0b00001,
  0b00010,
  0b00101,
  0b01010,
  0b00101,
  0b00010,
  0b00001,
  0b00000
},
{
  0b01000,
  0b00100,
  0b00010,
  0b00001,
  0b00010,
  0b00100,
  0b01000,
  0b00000
}
};

void createChars(LiquidCrystal_I2C lcd){
  for(uint8_t i = 0; i <= arrayLen; i++){
    lcd.createChar(i+1, customChar[i]);
  }
}

//lcd.createChar(1, customChar[1]);