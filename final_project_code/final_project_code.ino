
#include <Adafruit_NeoPixel.h>
#include <CapacitiveSensor.h>


// declaring vars

//neopixel vars
int constantGlowPin = 9;
int firingPin = 10;
int constantGlowAmt = 60;
int firingAmt = 20;

bool isFiring = false;

/* 
note: i'm sure there's a way to make this code a little more compact
using arrays but because the amounts of the pixels are different
i don't know how to make that work so i'm open to suggestions!!
*/

Adafruit_NeoPixel constantGlowStrip(constantGlowAmt, constantGlowPin);
Adafruit_NeoPixel firingStrip(firingAmt, firingPin);

uint32_t constantGold = constantGlowStrip.Color(255, 100, 0);
uint32_t firingGold = firingStrip.Color(255, 100, 0);


// button vars
int buttonPin1 = 13;
bool buttonState1 = false;
bool prevButtonState1 = false;

int buttonPin2 = 12;
bool buttonState2 = false;
bool prevButtonState2 = false;

// state vars for finite state machine
int state = 0;


void setup() {
  
  Serial.begin(9600);
  constantGlowStrip.begin();
  firingStrip.begin();
  constantGlowStrip.clear();
  firingStrip.clear();

}

void loop() {
  
  // constantGlowFill(constantGold);
  stateMachine();
  handleButton1();
  handleButton2();
  // firingSequence(firingGold, 10);
  // fire();

}

//////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////// functions /////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////

void handleButton1() {
  
  // reset the button states
  prevButtonState1 = buttonState1;
  buttonState1 = digitalRead(buttonPin1);
  
  // toggle if statement
  if (buttonState1 == HIGH && prevButtonState1 == LOW) {
    state += 1;
    
    if (state > 1) {
    	state = 0;
    }
    
    // debounce
    delay(50);
  }

}




void constantGlowFill(uint32_t c) {
  for (int i = 0; i<constantGlowStrip.numPixels(); i++) {
    constantGlowStrip.setPixelColor(i, c);
  }
  constantGlowStrip.show();
}


void firingSequence(uint32_t c, int interval) {

  static int index = 0;
  static unsigned long startTime = 0;

  firingStrip.clear();

  for (int i = 0; i<=firingStrip.numPixels(); i++) {
    firingStrip.setPixelColor(i, c);
  }

  if (millis() - startTime >= interval) {
    firingStrip.setBrightness(index);
    index++;
    // startTime = millis();


    if (index >= 255) {
     firingStrip.clear(); 
     index = 0;
     isFiring = false;
    }
  }

  firingStrip.show();

}

void handleButton2() {
  
  // reset the button states
  prevButtonState2 = buttonState2;
  buttonState2 = digitalRead(buttonPin2);
  
  // static bool isFiring = false;

  // toggle if statement
  if (buttonState2 == HIGH && prevButtonState2 == LOW) {
    isFiring = !isFiring;
    
    // debounce
    delay(50);
  }

      if (isFiring == true) {
    	firingSequence(firingGold, 10);
      // isFiring = !isFiring;
    }
    else if (isFiring == false) {
      firingStrip.clear();
      firingStrip.show();
    }
  
  Serial.println(isFiring);

}


void stateMachine() {

  switch (state) {
    case 0:
    constantGlowStrip.clear();
    firingStrip.clear();
    break;

    case 1:
    constantGlowFill(constantGold);
    // firingSequence(firingGold, 10);
    // fire();
    break;


  }

  // Serial.println(state);
  constantGlowStrip.show();
  // firingStrip.show();

}



