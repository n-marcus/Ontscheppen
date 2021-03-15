#define LED_PIN 2

const int NUM_LEDS = NUM_SOLENOIDS + 2;
CRGBArray<NUM_LEDS> leds;


void setupLEDs() {
  FastLED.addLeds<NEOPIXEL, LED_PIN>(leds, NUM_LEDS);
  for (int i = 0; i < NUM_LEDS; i ++) {
    leds[i] = CHSV(200, 255, 255);
    leds.fadeToBlackBy(40);
    FastLED.show();
    delay(40);
  }

  for (int i = 0; i < 100; i ++) { 
    leds.fadeToBlackBy(50);
    FastLED.show();
    delay(10);
  }
}

void updateLEDs() {
  for (int i = 0; i < NUM_SOLENOIDS; i ++) {
    if (solenoids[i]->getState()) {
      leds[i] = CHSV(i * (255/8), 255, 255);
    }
  }

  // do some periodic updates
  EVERY_N_SECONDS( 4 ) {
    checkLinkStatus();
    Serial.println("Checking link status " + String(linkStatus));
    if (linkStatus == 2 || linkStatus == 0) {
      Serial.println("Link Status = 0, Flashing red");
      fill_solid(leds, NUM_LEDS, CRGB::Green);
    } 
  }

  leds.fadeToBlackBy(50);
  FastLED.show();
}
