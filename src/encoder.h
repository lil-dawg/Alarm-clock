#include <Arduino.h>

void encoder_setup(int CLK, int DT, bool &lastStateCLK) {
        
	// Set encoder pins as inputs
	pinMode(CLK,INPUT);
	pinMode(DT,INPUT);

	// Read the initial state of CLK
	lastStateCLK = digitalRead(CLK);
}

char readEncoder(int CLK, int DT, bool &lastStateCLK){
  	// Read the current state of CLK
	bool currentStateCLK = digitalRead(CLK);

	char foo = 0;

	// If last and current state of CLK are different, then pulse occurred
	// React to only 1 state change to avoid double count
	if (currentStateCLK != lastStateCLK  && currentStateCLK == 1){

		// If the DT state is different than the CLK state then
		// the encoder is rotating CCW so return 1
		if (digitalRead(DT) != currentStateCLK) {
			foo = 1;
		} else {
			// Encoder is rotating CW so return 2
			foo = 2;
		}
	}

	// Remember last CLK state
	lastStateCLK = currentStateCLK;

	// Put in a slight delay to help debounce the reading
	delay(1);
	return foo;
}