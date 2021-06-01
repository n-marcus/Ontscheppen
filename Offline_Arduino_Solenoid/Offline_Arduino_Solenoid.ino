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
int maxFastTriggerTime = 100;
//how min fast is fast mode in ms between triggers
int minFastTriggerTime = 10;

int minDelTime = 200;
int maxDelTime = 2000;

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

//create an array of solenoid objects
Solenoid *solenoids[NUM_SOLENOIDS];

//name the pins that MOSFETs are connected to here
int solenoidPins[NUM_SOLENOIDS] = {2, 3, 4, 5};


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  delay(100);
  Serial.println("I am alive!");

  setupSolenoids();
}


void loop() {
  //20% chance of going into fast mode
  if (random(100) < 20) {
    fastMode = true;
    minDelTime = minFastTriggerTime;
    maxDelTime = maxFastTriggerTime;
    Serial.println("Fast mode is on");
  }

  //if we have reached the max amount of fast triggers, go back to normal mode
  if (fastTriggers > maxFastTriggers) {
    fastMode = false;
    Serial.println("Fast mode is off");
    minDelTime = 500;
    maxDelTime = 1000;
    fastTriggers = 0;
  }

  //pick a random solenoid to trigger
  solenoidIndex  = random(NUM_SOLENOIDS);
  Serial.println("About to randomly trigger solenoid " + String(solenoidIndex));

  //pick a random timing to wait after the trigger
  int delayTime = int(random(minDelTime, maxDelTime));
  Serial.println("Will wait for " + String(delayTime) + " seconds afterwards");

  //trigger solenoid for 25 ms
  solenoids[solenoidIndex]->trigger(25);
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
  if (random(100) < 5) {
    for (int i = 0; i < NUM_SOLENOIDS; i ++) {
      int delayTime = random(10, 50);
      for (int r = 0; r < 40; r ++) {
        solenoids[i]->trigger(delayTime);
        delay(delayTime + 5);
        solenoids[i]->update();
        delay(2);
      }
    }
  }



  cycleCount ++;
  //wait random time
  delay(delayTime);

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
