#include <PressButton.h>
#include <Arduino.h>

// I/O port allocations
const int BTN_OK = 11;
const int pinA = 2;
const int pinB = 3;

// Button
PressButton btnOk(BTN_OK);

bool prevPinA = false;
bool CWFlag = false;
bool CCWFlag = false;
char interruptDelay = 1;
unsigned long prevTime;
void interrupt();

void encoder_setup(int pinA, int pinB, int BTN_OK, bool &prevPinA) {
        
	// Set encoder pins as inputs
	pinMode(pinA, INPUT);
	pinMode(pinB, INPUT);

	// Save the initial state of pinA
	prevPinA = digitalRead(pinA);

	attachInterrupt(digitalPinToInterrupt(pinA), interrupt, CHANGE);
  attachInterrupt(digitalPinToInterrupt(pinB), interrupt, CHANGE);
}

char readEncoder(int pinA, int pinB, bool &lastStatepinA){
  	// Read the current state of pinA
	bool currentStatepinA = digitalRead(pinA);

	char foo = 0;

	// If last and current state of pinA are different, then pulse occurred
	// React to only 1 state change to avoid double count
	if (currentStatepinA != lastStatepinA  && currentStatepinA == 1){

		// If the pinB state is different than the pinA state then
		// the encoder is rotating CCW so return 1
		if (digitalRead(pinB) != currentStatepinA) {
			foo = 1;
		} else {
			// Encoder is rotating CW so return 2
			foo = 2;
		}
	}

	// Remember last pinA state
	lastStatepinA = currentStatepinA;

	// Put in a slight delay to help debounce the reading
	delay(1);
	return foo;
}
// rotary encoder interrupt
void interrupt(){
  if(millis() - prevTime > interruptDelay){
  bool pinStateA = digitalRead(pinA);
  if(pinStateA != prevPinA  && pinStateA == 1) {
    if (digitalRead(pinB) != pinStateA) {
			CCWFlag = true;
      CWFlag = false;
		} else {
			CWFlag = true;
      CCWFlag = false;
		}
  }
  prevPinA = pinStateA;
  prevTime = millis();
  }
}
void captureUserInput(){
  btnOk.CaptureDownState();
}