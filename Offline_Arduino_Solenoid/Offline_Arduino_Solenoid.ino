#define NUM_SOLENOIDS 4

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
  // put your main code here, to run repeatedly:

  if (waitDir) { 
    wait += waitIncr;
  } else {
    wait -= waitIncr;
  }
  if (wait > waitMax) { 
    waitDir = false;
  } else if (wait < waitMin) { 
    waitDir = true;
  }

  if (cycleCount % wait == 0) { 
    solenoidIndex ++;
    solenoidIndex = solenoidIndex % NUM_SOLENOIDS;
    solenoids[solenoidIndex]->trigger(40); 
  }

  for (int i = 0; i < NUM_SOLENOIDS; i ++) { 
    solenoids[i]->update();
  }

  cycleCount ++;
  delay(speed);
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
