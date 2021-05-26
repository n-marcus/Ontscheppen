#define LED_PIN 2
#define BRIGHTNESS  50




int ledHue = 150;

void setupLEDs() {
  FastLED.addLeds<NEOPIXEL, LED_PIN>(leds, NUM_LEDS);
  FastLED.setBrightness(BRIGHTNESS );
}

void updateLEDs() {
  for (int i = 0; i < NUM_SOLENOIDS; i ++) {
    if (solenoids[i]->getState()) {
      int solVel = solenoids[i]->maxOnTime * 2;
      solVel = solVel * solVel;
      solVel = constrain(solVel, 40, 255);
      Serial.println("lighting LED with vel " + String(solVel));
      leds[i] = CHSV(ledHue, solVel, solVel);
    }
  }

  //blink the eyes to show activity
  if (lastOscMessage < 10) {
    leds[NUM_LEDS] = CHSV(ledHue, 255, 255);
    leds[NUM_LEDS - 1] = CHSV(ledHue, 255, 255);
  }

  leds.fadeToBlackBy(50);
  FastLED.show();
}
