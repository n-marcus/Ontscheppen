#include <FastLED.h>

//how many solenoids are connected?
#define NUM_SOLENOIDS 4

//add two for eyes
const int NUM_LEDS = NUM_SOLENOIDS + 2;
CRGBArray<NUM_LEDS> leds;

int speed = 50;

int wait = 500;

int waitMin = 10;

int waitMax = 5000;

int waitIncr = 1;

bool waitDir = false;

long cycleCount = 0;

int flam = 5;

int solenoidIndex = 0;

long lastTrigger = 0;

//are we going fast?
bool fastMode = false;
//how many triggers will we have in fast mode?
int maxFastTriggers = 25;
//how many triggers have we had in fastmode?
int fastTriggers = 0;
//how max fast is fast mode? in ms between triggers
int maxFastTriggerTime = 20;
//how min fast is fast mode in ms between triggers
int minFastTriggerTime = 10;

// spaarzame modus minimale en maximale wachttijd tussen triggers
int minDelTimeBase = 1000;
int maxDelTimeBase = 5000;

int minDelTime = minDelTimeBase;
int maxDelTime = maxDelTimeBase;

//this is the chance that we will go in chance mode 0 = 0% 1000 = 100%, so 1 = 0.1%
int percentageFastMode = 2;

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
        //        Serial.println("Solenoid on pin " + String(pin) + " is now off because " + String(millis() - lastTriggerTime) + " >= " + String(maxOnTime));
      }
    }

    virtual void trigger(int timing) {
      //triggers the solenoid
      maxOnTime = timing;
      //only retrigger if the solenoid is already off
      if (state == false) {
        digitalWrite(pin, HIGH);
        lastTriggerTime = millis(); //when was I last triggered?
        state = true; //now the solenoid is on
        Serial.println("Solenoid on pin " + String(pin) + " is now on with time " + String(lastTriggerTime));
      }
    }

    virtual void setPin(int newPin) {
      //change pin of this solenoid
      pin = newPin;
    }

    virtual int getPin() {
      return pin;
    }

    virtual bool getState() {
      return state;
    }
};

//name the pins that MOSFETs are connected to here
//Arduino with 4 Solenoids pins  {2, 3, 4, 5,}
//Arduino with 8 Solenoids pins  {2, 3, 4, 5, 16, 17, 18, 19}

int solenoidPins[NUM_SOLENOIDS] = {2, 3, 4, 5};
//int solenoidPins[NUM_SOLENOIDS] = {2, 3, 4, 5, 16, 17, 18, 19};


//create an array of solenoid objects
Solenoid *solenoids[NUM_SOLENOIDS];



void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  delay(100);
  Serial.println("I am alive!");

  setupSolenoids();
  setupButton();
  setupLEDs();
}


void loop() {

  //20% chance of going into fast mode
  if (random(1000) < percentageFastMode) {
    fastMode = true;
    minDelTime = minFastTriggerTime;
    maxDelTime = maxFastTriggerTime;
    Serial.println("Fast mode is on");
  }

  //if we have reached the max amount of fast triggers, go back to normal mode
  if (fastTriggers > maxFastTriggers) {
    fastMode = false;
    Serial.println("Fast mode is off");
    minDelTime = minDelTimeBase;
    maxDelTime = maxDelTimeBase;
    fastTriggers = 0;
  }

  //pick a random solenoid to trigger
  solenoidIndex  = random(NUM_SOLENOIDS);
  Serial.println("About to randomly trigger solenoid " + String(solenoidIndex));

  //pick a random timing to wait after the trigger
  int delayTime = int(random(minDelTime, maxDelTime));
  Serial.println("Will wait for " + String(delayTime) + " ms afterwards");

  //trigger solenoid for 25 ms
  solenoids[solenoidIndex]->trigger(25);
  updateLEDs();
  if (fastMode) {
    fastTriggers ++; //count how many fast triggers we've had
  }

  //update every solenoid
  for (int i = 0; i < NUM_SOLENOIDS; i ++) {
    solenoids[i]->update();
  }

  //turn LEDs on or off accordingly
  updateLEDs();


  //5% chance to get into repeat mode
  if (random(1000) < percentageFastMode) {
    Serial.println("Repeat mode!");
    for (int i = 0; i < NUM_SOLENOIDS; i ++) {
      int delayTime = random(10, 50);
      int repeats = maxFastTriggers / 4;
      
      for (int r = 0; r < repeats; r ++) {
        Serial.println("Repeat mode triggering solenoid " + String(i));
        solenoids[i]->trigger(delayTime);
        updateLEDs();
        delay(delayTime + 5);
        solenoids[i]->update();
        updateLEDs();
        delay(delayTime);
      }
    }
  }
  updateLEDs();


  cycleCount ++;
  //wait random time
  long startTime = millis();
  long endTime = startTime + delayTime;
  Serial.println("Waiting for " + String(delayTime) + "ms...");
  while (millis() < endTime) {
    updateButton();
    updateLEDs();
    delay(2);

  }

  updateLEDs();

}
