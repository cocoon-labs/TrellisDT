// Run this function upon switching to "presets" screen
void specsSetup() {
  wipe();
  populateSpecs();
  trellis.writeDisplay();
}

void populateSpecs() {
  for (uint8_t i = 0; i < min(nSpecOptions, 8); i++) {
    setXY(i, i);
  }
}

void specsLoop() {
  if (trellis.readSwitches()) {
    // go through every color preset
    for (uint8_t i = 0; i < min(nSpecOptions, 8); i++) {
      // if it was pressed, send color preset ID and exit loop
      if (justPressedXY(i, i)) {
        sendOptionSelect(i);
        break;
      }
    }
  }
}

void sendOptionSelect(uint8_t i) {
  Serial.write(i);
}
