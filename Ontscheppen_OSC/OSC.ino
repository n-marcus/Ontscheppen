byte mac[] = {0xCA, 0xFE, 0xCA, 0xFE, 0xCA, 0xFE};
IPAddress thisIp(169, 254, 143, 3);
unsigned int thisPort = 8888;
IPAddress thatIp(169, 254, 88, 3);
unsigned int thatPort = 9002;

EthernetUDP Udp;
char packetBuffer[UDP_TX_PACKET_MAX_SIZE];

void setupOSC() {
  //start an ethernet connection with the given IP and mac adresses
  Ethernet.begin(mac, thisIp);
  Udp.begin(thisPort);
  delay(500);
  Serial.println("Hey! My IP is:");
  Serial.println(Ethernet.localIP());
}


void getOSC() {


  // important! non-blocking listen routine
//  etherOSC.listen();  // if there is data waiting, this will trigger OSC EVENT

  OSCBundle bundleIN;
  //  Serial.println("Getting osc");
  int size;
  if ((size = Udp.parsePacket()) > 0 ) {
    Serial.println("Got something!");
    int defSize = size;
    byte incoming[size];
    while (size--) {
      byte inByte = Udp.read();
      incoming[defSize - size - 1] = inByte;
    }
    //print everything that is received
    for (int i = 0; i < defSize; i++) {
      //      Serial.print("i: ");
      //      Serial.print(i);
      //      Serial.print(", ");
      Serial.print(incoming[i]);

      //it prints ,ii to denote that it's 2 integers 32bit int32_t in arduino
    }

    Serial.println("");

    if (matchPattern(incoming, "/test") > 0) {
      Serial.print("Test data:");
      Serial.println(incoming[defSize - 1]);
    }

    if (matchPattern(incoming, "/solenoid") > 0) {
      //      Serial.println("We got a solenoid trigger message");
      int solNum = incoming[defSize - 1];
      solNum = solNum % NUM_SOLENOIDS;
      Serial.println("Triggering solenoid number " + String(solNum) + " after OSC message");
      if (solNum < NUM_SOLENOIDS && solNum >=  0) {
        //TODO find a way to parse multiple arguments in OSC message to also have velocity

        solenoids[solNum]->trigger(50);
      }
    }
  }
}


int matchPattern(byte *arr, String pattern) {
  //function to check if an OSC pattern matches a query
  unsigned int counter = 0;
  for (unsigned int i = 0; i < pattern.length(); i++) {
    if (char(arr[i]) == char(pattern[i])) {
      counter++;
    }
  }
  if (counter == pattern.length()) {
    return pattern.length();
  }
  else {
    return 0;
  }
}

void sendOSC(char* addr, int val) {
  OSCMessage msg(addr);
  msg.add((int32_t)val);
  Udp.beginPacket(thatIp, thatPort);
  msg.send(Udp);
  Udp.endPacket();
  msg.empty();
}
