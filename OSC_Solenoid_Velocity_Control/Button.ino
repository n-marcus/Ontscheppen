#define BUTTON_PIN 1

void setupButton(){
  pinMode(BUTTON_PIN, INPUT_PULLUP);

  
}

void updateButton(){
  int buttonRead = digitalRead(BUTTON_PIN);
  if(!buttonRead ){
    leds[1] = CRGB::Yellow;
    testSolenoids();
  }
  
}
