/*
  Example for receiving

  https://github.com/suda/RCSwitch

  If you want to visualize a telegram copy the raw data and
  paste it into http://test.sui.li/oszi/

  Connect receiver to D2 pin and open serial connection @ 9600 bauds
*/

#include "RCSwitch/RCSwitch.h"

RCSwitch mySwitch = RCSwitch();
int ledPin = D7;
int inputPin = D2;

void output(unsigned long long decimal, unsigned int length, unsigned int delay, unsigned int* raw, unsigned int protocol) {

  if (decimal == 0) {
    Serial.print("Unknown encoding.");
  } else {
    char* b = mySwitch.dec2binWzerofill(decimal, length);

    Serial.print("Decimal top 32 bits: ");
    Serial.print((unsigned long)(decimal >> 32));
    Serial.print(" bottom 32 bits: ");
    Serial.println((unsigned long)(decimal));

    Serial.print("Hex: ");
    Serial.print((unsigned long)(decimal >> 32), HEX);
    Serial.println((unsigned long)(decimal), HEX);
    
    Serial.print(" (");
    Serial.print( length );
    Serial.print("Bit) Binary: ");
    Serial.print( b );
    Serial.print(" PulseLength: ");
    Serial.print(delay);
    Serial.print(" microseconds");
    Serial.print(" Protocol: ");
    Serial.println(protocol);
  }

  Serial.print("Raw data: ");
  for (int i=0; i<= length*2; i++) {
    Serial.print(raw[i]);
    Serial.print(",");
  }
  Serial.println();
  Serial.println();
}

void setup() {
  Serial.begin(9600);
  Serial.println("Listening");

  pinMode(inputPin, INPUT_PULLDOWN);
  mySwitch.enableReceive(inputPin);

  pinMode(ledPin, OUTPUT);
}

void loop() {
  int inputPinState = digitalRead(inputPin);
  digitalWrite(ledPin, inputPinState);

  if (mySwitch.available()) {
    mySwitch.disableReceive();
    output(mySwitch.getReceivedValue(), mySwitch.getReceivedBitlength(), mySwitch.getReceivedDelay(), mySwitch.getReceivedRawdata(), mySwitch.getReceivedProtocol());
    mySwitch.enableReceive(0);
    mySwitch.resetAvailable();
  }
}
