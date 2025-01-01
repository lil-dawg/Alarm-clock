#ifndef Menu20x4_h
#define Menu20x4_h

#include <Arduino.h>

class Menu20x4 {
	public:
		Menu20x4(int buttonPin, int encPinA, int encPinB);
	private:
		bool updateAllItems;
		bool updateItemValue;
		unsigned char itemCnt;
		unsigned char pntrPos;
		unsigned char dispOffset;
		unsigned char root_pntrPos = 1;
		unsigned char root_dispOffset = 0;
		unsigned char flashCntr;
		bool flashIsOn;
};

#endif