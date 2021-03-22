#define LED_PIN 2

const int NUM_LEDS = NUM_SOLENOIDS + 2;
CRGBArray<NUM_LEDS> leds;

int ledHue = 150;

void setupLEDs() {
  FastLED.addLeds<NEOPIXEL, LED_PIN>(leds, NUM_LEDS);
  for (int i = 0; i < NUM_LEDS; i ++) {
    leds[i] = CHSV(ledHue, 255, 255);
    leds.fadeToBlackBy(40);
    FastLED.show();
    delay(40);
  }

  for (int i = 0; i < 200; i ++) { 
    leds.fadeToBlackBy(50);
    FastLED.show();
    delay(10);
  }
}

void updateLEDs() {
  for (int i = 0; i < NUM_SOLENOIDS; i ++) {
    if (solenoids[i]->getState()) {
      int solVel = solenoids[i]->maxOnTime * 2;
      solVel = constrain(solVel, 40, 255);
      Serial.println("lighting LED with vel " + String(solVel));
      leds[i] = CHSV(ledHue, solVel, solVel);
    }
  }
  leds.fadeToBlackBy(50);
  FastLED.show();
}
