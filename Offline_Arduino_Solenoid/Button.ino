#define BUTTON_PIN 14

void setupButton() {
  pinMode(BUTTON_PIN, INPUT_PULLUP);
}

void updateButton() {
  int buttonRead = digitalRead(BUTTON_PIN);
  if (!buttonRead ) {
    fill_solid(leds, NUM_LEDS, CRGB::Black);
    leds[NUM_LEDS - 1] = CRGB::Blue;
    leds[NUM_LEDS - 2] = CRGB::Blue;
    testSolenoids();
  }
}
