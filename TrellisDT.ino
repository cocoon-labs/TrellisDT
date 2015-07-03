#include <Wire.h>
#include "Adafruit_Trellis.h"

#define numKeys 64

// mode options
#define SCREENSAVER 0
#define PRESETS 1
#define SPECS 2
#define ARCADE 3

// screensaver mode options
#define GOL 0
#define RINGS 1

// Connect Trellis Vin to 5V and Ground to ground.
// Connect the INT wire to pin #A2 (can change later!)
#define INTPIN A2
// Connect I2C SDA pin to your Arduino SDA line
// Connect I2C SCL pin to your Arduino SCL line

Adafruit_Trellis matrix0 = Adafruit_Trellis();
Adafruit_Trellis matrix1 = Adafruit_Trellis();
Adafruit_Trellis matrix2 = Adafruit_Trellis();
Adafruit_Trellis matrix3 = Adafruit_Trellis();

Adafruit_TrellisSet trellis =  Adafruit_TrellisSet(&matrix0, &matrix1, &matrix2, &matrix3);

uint8_t mode = SCREENSAVER;
uint8_t ssMode = RINGS;
uint8_t ssModeCount = 2;
uint8_t ssDelay = 100; // 100ms delay is minimum
uint8_t randFactor = 100;


void setup() {
  Serial.begin(9600);
  Serial.println("Trellis Demo");

  // INT pin requires a pullup
  pinMode(INTPIN, INPUT);
  digitalWrite(INTPIN, HIGH);
  randomSeed(analogRead(0));
  
  // begin() with the addresses of each panel in order
  trellis.begin(0x70, 0x71, 0x72, 0x73);  // or four!

  //drawRows();
  wipe();
  //golSetup();
}


void loop() {
  delay(30); // 30ms delay is required, dont remove me!
  
  switch(mode) {
    case SCREENSAVER:
      ssLoop();
      break;
    case PRESETS:
      
      break;
    case SPECS:
      
      break;
    case ARCADE:
      
      break;
  }
}

void ssLoop() {
  ssRandomize();
  switch(ssMode) {
    case GOL:
      golLoop();
      break;
    case RINGS:
      ringsLoop();
      break;
  }
}

void ssRandomize() {
  if (random(randFactor) == 0) {
    uint8_t newSSMode = random(ssModeCount);
    if (ssMode != newSSMode) {
      ssMode = newSSMode;
      wipe();
    }
  }
  if (random(randFactor) == 0) {
    ssDelay = random(400) + 100;
  }
}

uint8_t unmorph(uint8_t i) {
  uint8_t newI;
  if (i < 16) {
    uint8_t r = i / 4;
    newI = i + (r * 4);
  } else if (i < 32) {
    i -= 16;
    uint8_t r = i / 4;
    newI = i + 4 + (r * 4);
  } else if (i < 48) {
    i -= 32;
    uint8_t r = i / 4;
    newI = i + 36 + (r * 4);
  } else {
    i -= 48;
    uint8_t r = i / 4;
    newI = i + 32 + (r * 4);
  }
  return newI;
}

uint8_t morph(uint8_t i) {
  uint8_t newI;
  uint8_t r = i / 8;
  uint8_t c = i % 8;
  if (i < 32) {
    if (c < 4) {
      newI = (4 * r) + c;
    } else {
      c -= 4;
      newI = (4 * r) + c + 16;
    }    
  } else {
    if (c < 4) {
      r -= 4;
      newI = (4 * r) + c + 48;
    } else {
      r -= 4;
      c -= 4;
      newI = (4 * r) + c + 32;
    }
  }
  return newI;
}

uint8_t morphXY(uint8_t x, uint8_t y) {
  return morph((8 * y) + x);
}

void setXY(uint8_t x, uint8_t y) {
  trellis.setLED(morphXY(x, y));
}

void clrXY(uint8_t x, uint8_t y) {
  trellis.clrLED(morphXY(x, y));
}

void toggle(uint8_t placeVal) {
  if (trellis.isLED(placeVal))
    trellis.clrLED(placeVal);
  else
    trellis.setLED(placeVal);
}

void wipe() {
  for (uint8_t i=0; i<numKeys; i++) {
    trellis.clrLED(i);
  }
  trellis.writeDisplay();
}

void setAllOff() {
  for (uint8_t i=0; i<numKeys; i++) {
    trellis.clrLED(i);
  }
}

void drawRows() {
  for (uint8_t y=0; y<8; y++) {
    for (uint8_t x=0; x<8; x++) {
      trellis.setLED(morphXY(x, y));
      trellis.writeDisplay();
      delay(50);
    }
  }
}

void setRow(uint8_t y) {
  for (uint8_t x=0; x<8; x++) {
    trellis.setLED(morphXY(x, y));
  }
}

void setCol(uint8_t x) {
  for (uint8_t y=0; y<8; y++) {
    trellis.setLED(morphXY(x, y));
  }
}

void setRowPart(uint8_t y, uint8_t minim, uint8_t maxim) {
  for (uint8_t x=minim; x<=maxim; x++) {
    trellis.setLED(morphXY(x, y));
  }
}

void setColPart(uint8_t x, uint8_t minim, uint8_t maxim) {
  for (uint8_t y=minim; y<=maxim; y++) {
    trellis.setLED(morphXY(x, y));
  }
}

void toggleRow(uint8_t y) {
  for (uint8_t x=0; x<8; x++) {
    toggle(morphXY(x, y));
  }
}

void toggleCol(uint8_t x) {
  for (uint8_t y=0; y<8; y++) {
    toggle(morphXY(x, y));
  }
}

boolean isEmpty() {
  for (uint8_t i=0; i<numKeys; i++) {
    if (trellis.isLED(i)) {
      return false;
    }
  }
  return true;
}

boolean toggleModeLoop() {
  if (trellis.readSwitches()) {
    // go through every button
    for (uint8_t i=0; i<numKeys; i++) {
      // if it was pressed...
      if (trellis.justPressed(i)) {
	// Alternate the LED
	if (trellis.isLED(i))
	  trellis.clrLED(i);
	else
	  trellis.setLED(i);
      } 
    }
    // tell the trellis to set the LEDs we requested
    trellis.writeDisplay();
  }
}