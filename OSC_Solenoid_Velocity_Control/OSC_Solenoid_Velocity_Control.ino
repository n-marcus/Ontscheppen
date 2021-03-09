
// MESSAGE PROTOCOL OBJECT
#include <OscUDP.h>
// hardware libraries to access use the shield
#include <SPI.h>
#include <Ethernet.h>
#include <EthernetUdp.h>


// OUR OSC MESSAGE OBJECT
OscUDP etherOSC;


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
        digitalWrite(LED_BUILTIN, LOW);
        //        Serial.println("Solenoid on pin " + String(pin) + " is now off because " + String(millis() - lastTriggerTime) + " >= " + String(maxOnTime));
      }
    }

    virtual void trigger(int timing) {
      //triggers the solenoid
      maxOnTime = timing;
      //only retrigger if the solenoid is already off
      if (state == false) {
        digitalWrite(pin, HIGH);
        digitalWrite(LED_BUILTIN, HIGH);
        lastTriggerTime = millis(); //when was I last triggered?
        state = true; //now the solenoid is on
        Serial.println("Solenoid on pin " + String(pin) + " is triggered at time " + String(lastTriggerTime));
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

void setup() {
  Serial.begin(15200);
  delay(500);
  Serial.println("I am alive!");
  pinMode(LED_BUILTIN, OUTPUT);

  setupMIDI();
  setupSolenoids();
  delay(200);
  //  testSolenoids();
  setupOSC();
}

void loop() {
  updateSolenoids();
  //get incoming MIDI
  usbMIDI.read();
  // important! non-blocking listen routine
  etherOSC.listen();  // if there is data waiting, this will trigger OSC EVENT

  //  OSCHeartBeat();
}
