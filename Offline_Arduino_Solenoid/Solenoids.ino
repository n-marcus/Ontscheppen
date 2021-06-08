
void setupSolenoids() {
  //this function creates an array of solenoid classes and sets their output pins to the right physical pins based on the solenoidPins array
  Serial.println("Creating solenoid classes!");
  for (int i = 0; i < NUM_SOLENOIDS; i ++) {
    solenoids[i] = new Solenoid();
    Serial.println("Solenoid " + String(i) + " has " + String(solenoidPins[i]) + " as output pin");
    pinMode(solenoidPins[i], OUTPUT);
    solenoids[i]->setPin(solenoidPins[i]);
  }
}

void testSolenoids() {
  Serial.println("Starting a solenoid testing sequence");
  delay(10);//wait till all solenoids have fired
  for (int i = 0; i < NUM_SOLENOIDS; i ++)  {
    Serial.println("Now test firing solenoid " + String(i) + " on pin " +  String(solenoids[i]->getPin()));
    solenoids[i]->trigger(30);
    delay(40);
    solenoids[i]->update(); //call update to discharge the solenoid
    leds[i] = CHSV(ledHue, 255, 255);
    FastLED.show();
    delay(100);
  }
  Serial.println("Testing done");
}

void updateSolenoids() { 
  for (int i =0 ; i < NUM_SOLENOIDS; i ++) { 
    solenoids[i]->update();
  }
}
