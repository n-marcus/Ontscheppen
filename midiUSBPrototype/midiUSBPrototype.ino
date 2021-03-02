#define NUM_SOLENOIDS 16

//Solenoid class
class Solenoid {
    //a class to describe how a solenoid works
  public:
    unsigned long lastTriggerTime = 0;
    unsigned int maxOnTime = 10; //how long can this Solenoid be on?
    bool state = false; //is this solenoid triggered right now?
    int pin = 0;
    virtual void update() {
      //if I am on and the last time I was triggered is too long ago, turn off
      if (state &&  millis() -  lastTriggerTime >= maxOnTime) {
        state = false;
        digitalWrite(pin, LOW);
        Serial.println("Solenoid on pin " + String(pin) + " is now off because " + String(millis() - lastTriggerTime) + " >= " + String(maxOnTime));
      }
    }

    virtual void trigger(int timing) {
      //triggers the solenoid
      maxOnTime = timing;
      if (state == false) {
        digitalWrite(pin, HIGH);
        lastTriggerTime = millis(); //when was I last triggered?
        state = true;
        Serial.println("Solenoid on pin " + String(pin) + " is now on with time " + String(lastTriggerTime));
      }
    }

    virtual void setPin(int newPin) {
      pin = newPin;
    }

    virtual int getPin() {
      return pin;
    }

    virtual bool getState() {
      return state;
    }
};

Solenoid *solenoids[NUM_SOLENOIDS];

int solenoidPins[NUM_SOLENOIDS] = {3,4,5,6,7,8,};

void setup() {
  Serial.begin(115200);
  delay(500);
  Serial.println("I am alive!");
  pinMode(LED_BUILTIN, OUTPUT);
  usbMIDI.setHandleNoteOff(OnNoteOff);
  usbMIDI.setHandleNoteOn(OnNoteOn);

  for (int i = 0; i < NUM_SOLENOIDS; i ++) {
    solenoids[i] = new Solenoid();
    Serial.println("Solenoid " + String(i) + " has " + String(solenoidPins[i]) + " as output pin");
    pinMode(solenoidPins[i], OUTPUT);
    solenoids[i]->setPin(solenoidPins[i]);
  }


}

void loop() {
  //get incoming MIDI
  usbMIDI.read();

  //check all solenoids and if they should be on or off at this moment in time;
  for (int i = 0; i < NUM_SOLENOIDS; i ++ ) {
    //    Serial.println("Checking if solenoid " + String(i) + " should be on or off");
    solenoids[i]->update();
    //    Serial.println("it is now " + String(solenoids[i]->getState()));
  }
}

void OnNoteOn(byte channel, byte note, byte velocity) {
  digitalWrite(LED_BUILTIN, HIGH); // Any Note-On turns on LED
  Serial.println("Noteon note:" + String(note) + " vel:" + String(velocity));
  solenoidPulse(note, velocity); //trigger a solenoid
}

void OnNoteOff(byte channel, byte note, byte velocity) {
  digitalWrite(LED_BUILTIN, LOW); // Any Note-On turns on LED
  Serial.println("Noteoff note:" + String(note) + " vel:" + String(velocity));
}

void solenoidPulse(int note, byte velocity) { //note is 0-11
  //this function makes a solenoid pulse and stores it in an array to keep track of which solenoids are triggered or not
  int solenoid = note % NUM_SOLENOIDS;
  int timing = (float(velocity) / 128.) * 50;
  Serial.println("Triggering solenoid " + String(solenoid) + " on pin " + String(solenoids[solenoid]->getPin()) + " with timing " + String(timing));

  solenoids[solenoid]->trigger(timing);
}