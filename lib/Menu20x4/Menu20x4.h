#ifndef Menu20x4_h
#define Menu20x4_h

#include <Arduino.h>

class Menu20x4 {
	public:
		Menu20x4(int buttonPin, int encPinA, int encPinB);
	private:
		bool updateAllItems;
		bool updateItemValue;
		uint8_t itemCnt;
		uint8_t pntrPos;
		uint8_t dispOffset;
		uint8_t root_pntrPos = 1;
		uint8_t root_dispOffset = 0;
		uint8_t flashCntr;
		bool flashIsOn;
};

#endif