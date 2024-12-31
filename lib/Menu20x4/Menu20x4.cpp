#include <Arduino.h>
#include <Menu20x4.h>
#include <liquidCrystal_I2C.h>

Menu20x4::Menu20x4(int buttonPin, int encPinA, int encPinB) {
	LiquidCrystal_I2C lcd(0x27, 20, 4);
}