

//name the pins that MOSFETs are connected to here
int solenoidPins[NUM_SOLENOIDS] = {1, 2, 3, 4, 5, 6, 7, 8, 23, 22, 21, 20, 19, 18, 17, 16};

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
  delay(100);//wait till all solenoids have fired
  for (int i = 0; i < NUM_SOLENOIDS; i ++)  {
    solenoids[i]->trigger(50);
    Serial.println("Now test firing solenoid " + String(i) + " on pin " +  String(solenoids[i]->getPin()));
    delay(100);
  }
  Serial.println("Testing done");
}
