// Teensy 3.2 ethernet wiring:
// Ethernet adapter label -> Teensy port number
// SCK  ->  13
// MI   ->  12
// MO   ->  11
// CS   ->  10
// RST  ->  9
// V    ->  3.3v
// G    ->  GND

// setup a UDP object
EthernetUDP UDP;

// Enter a MAC address and IP address for your SHIELD.
// The IP address will be dependent on your local network
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };

// listeningIP ==  SHIELD initialization IP
IPAddress listeningIP(169, 254, 143, 3); // you need to set this

// listening -- port
unsigned int listeningPort = 8888;      // local port to listen on


// speaking
// set up our destination NODE
NetAddress destination;
IPAddress destinationIP(  169, 254, 233, 100 );
int destinationPort = 8080;

elapsedMillis lastHeartBeat;
int heartRate = 1000;

void setupOSC() {
  // start ethernet on the shield
  Ethernet.begin(mac, listeningIP);

  // start UDP object, listening on port listeningPort
  UDP.begin(listeningPort);

  // set up our communication protocol
  // pass the UDP object to OSC
  etherOSC.begin(UDP);

  // define our destination location
  destination.set(destinationIP, destinationPort);

  delay(500);
  Serial.println("Hey! My IP is:");
  Serial.println(Ethernet.localIP());

  Serial.println("I will be sending stuff to:");
  Serial.println(String(destinationIP) + " and port: " + String(destinationPort));
}

void OSCHeartBeat() {
  if (lastHeartBeat > heartRate) {
    //    Serial.println("Sending heartbeat!");
    // it seems that if the host cannot be found the UDP.beginPacket() function is blocked and stalls the Teensy completely for a few seconds
    //    create an OSC message
    //    OscMessage msg("/h");
    //    msg.add("b");
    //    UDP.beginPacket(destinationIP, destinationPort);
    //    msg.send(UDP);
    //    UDP.endPacket();
    //    msg.empty();
    //
    //    UDP.beginPacket(destinationIP, destinationPort);
    //
    //    UDP.write("h");
    //    UDP.endPacket();

    //    sendOSC("heartBeat", 1);

    //reset the heartbeat timer
    lastHeartBeat = 0;
  }
}

void oscEvent(OscMessage &m) { // *note the & before msg
  // receive a message
  char address[20];
  m.getAddress(address, 0);
  Serial.print("Received OSC message on adress:");
  Serial.print(address);
  Serial.println("");

  m.plug("/solenoid", solenoidOSC);
}

void solenoidOSC(OscMessage &m) {
//  Serial.println("Received solenoid OSC message");
  char address[20];
  m.getAddress(address, 0);
  int sol = m.getInt(0);
  sol = sol % NUM_SOLENOIDS;
  int vel = m.getInt(1);
  vel = constrain(vel, 0, 100);
  if (vel > 0) {
    Serial.println("Triggering solenoid " + String(sol) + " with velocity: " + String(vel));
    solenoids[sol]->trigger(vel);
  } else {
    //    Serial.println("Velocity is 0, ignoring this");
  }
}
