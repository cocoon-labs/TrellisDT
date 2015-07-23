// Run this function upon switching to "presets" screen
void presetsSetup() {
  wipe();
  populatePresets();
  trellis.writeDisplay();
}

void populatePresets() {
  for (uint8_t i = 0; i < nColorPresets; i++) {
    setXY(i/8, i%8);
  }
  for (uint8_t i = 0; i < nModePresets; i++) {
    setXY(7 - i/8, i%8);
  }
}

void presetsLoop() {
  if (trellis.readSwitches()) {
    // go through every color preset
    for (uint8_t i=0; i<nColorPresets; i++) {
      // if it was pressed, send color preset ID and exit loop
      if (justPressedXY(i/8, i%8)) {
        sendColor(i);
        break;
      }
    }
    // go through every mode preset
    for (uint8_t i=0; i<nModePresets; i++) {
      // if it was pressed, send mode preset ID and exit loop
      if (justPressedXY(7 - i/8, i%8)) {
        sendMode(i);
        break;
      }
    }
  }
}

void sendColor(uint8_t c) {
  Serial.write(COLORPRESET);
  Serial.write(c);
}

void sendMode(uint8_t m) {
  Serial.write(MODEPRESET);
  Serial.write(m);
}
