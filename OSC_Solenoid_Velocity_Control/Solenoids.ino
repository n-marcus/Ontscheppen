

//name the pins that MOSFETs are connected to here
//the order is important in combination with the wiring of the Teensy
int solenoidPins[NUM_SOLENOIDS] = {16, 17, 18, 19, 20, 21, 22, 23, 3, 4, 5, 6, 7, 8, 15, 14};


void solenoidPulse(int note, byte velocity) { //note is 0-11
  //this function makes a solenoid pulse and stores it in an array to keep track of which solenoids are triggered or not
  int solenoid = note % NUM_SOLENOIDS;
  //TODO make timing work with microSeconds for a higher velocity resolution
  int timing = (float(velocity) / 128.) * 50; //calculate velocity, from 0 - 128 to 0 - 50ms for the timing
  Serial.println("Triggering solenoid " + String(solenoid) + " on pin " + String(solenoids[solenoid]->getPin()) + " with timing " + String(timing));

  //trigger the class and fire the physical solenoid
  solenoids[solenoid]->trigger(timing);
}

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
  for (int i = 0 ; i < NUM_SOLENOIDS; i ++) {
    solenoids[i]->update();
  }
}
