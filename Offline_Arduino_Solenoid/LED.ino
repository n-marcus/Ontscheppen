#define LED_PIN 11
#define BRIGHTNESS  50

int eyeLED = 0;

int ledHue = 150;

void setupLEDs() {
  Serial.println("Setting up LEDS");
  FastLED.addLeds<NEOPIXEL, LED_PIN>(leds, NUM_LEDS);
  FastLED.setBrightness(BRIGHTNESS );
}

void updateLEDs() {
  for (int i = 0; i < NUM_SOLENOIDS; i ++) {
    if (solenoids[i]->getState()) {
      int solVel = solenoids[i]->maxOnTime * 2;
      solVel = solVel * solVel;
      solVel = constrain(solVel, 40, 255);
//      Serial.println("lighting LED with vel " + String(solVel));
      leds[i] = CHSV(ledHue, solVel, solVel);
      if (!fastMode) {
        int eyeToBlink = (NUM_LEDS - 2) + eyeLED;
//        Serial.println("Blinking eye " + String(eyeLED));
        leds[eyeToBlink] = CRGB::Blue;
        eyeLED += 1;
        eyeLED %= 2;
      } else {
        leds[NUM_LEDS - 1] = CRGB::Green;
        leds[NUM_LEDS - 2] = CRGB::Green;
      }
    }
  }

  leds.fadeToBlackBy(10);
  FastLED.show();
}
