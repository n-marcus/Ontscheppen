
void OnNoteOn(byte channel, byte note, byte velocity) {
  digitalWrite(LED_BUILTIN, HIGH); // Any Note-On turns on LED
  Serial.println("Noteon note:" + String(note) + " vel:" + String(velocity));
  solenoidPulse(note, velocity); //trigger a solenoid
  lastOscMessage = 0; //reset last message counter
}

void OnNoteOff(byte channel, byte note, byte velocity) {
  digitalWrite(LED_BUILTIN, LOW); // Any Note-On turns on LED
  Serial.println("Noteoff note:" + String(note) + " vel:" + String(velocity));
  lastOscMessage = 0; //reset last message counter
}

void setupMIDI() {
  usbMIDI.setHandleNoteOff(OnNoteOff);
  usbMIDI.setHandleNoteOn(OnNoteOn);
}
