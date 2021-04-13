#include "Arduino.h"
#include <SPI.h>
#include <RF24.h>
#include "printf.h"

#define DEBUG false
#define KEEP_ALIVE_CIRCUIT_ENABLED true
#define KEEP_ALIVE_ON_INTERVAL 1000
#define KEEP_ALIVE_OFF_INTERVAL 20000

const int keepAlivePin = 1;
const int pedalLeftPin = 9;
const int pedalMiddlePin = 8;
const int pedalRightPin = 10;
const int CEPin = A1;
const int CSPin = A0;
const int pedalPin [] = {pedalLeftPin, pedalMiddlePin, pedalRightPin};

const int PEDAL_COUNT = ( sizeof(pedalPin) / sizeof(int) );

boolean currentPedalVal[PEDAL_COUNT];

boolean isKeepAlive = false;
long keepAliveTimer = 0;

RF24 radio(CEPin, CSPin);
byte addresses[][6] = {"1Node", "2Node"};

void setup() {

  for (int i = 0; i < PEDAL_COUNT; i++) {
    pinMode(pedalPin[i], INPUT_PULLUP);
  }
  
  pinMode(keepAlivePin, OUTPUT);

  if (DEBUG) {
    Serial.begin(115200);
  }

  // Initiate the radio object
  radio.begin();
  // Set the transmit power to lowest available to prevent power supply related issues
  radio.setPALevel(RF24_PA_MIN);
  // Set the speed of the transmission to the quickest available
  radio.setDataRate(RF24_2MBPS);
  // Use a channel unlikely to be used by Wifi, Microwave ovens etc
  radio.setChannel(124);

  // Open a writing and reading pipe on each radio, with opposite addresses
  radio.openWritingPipe(addresses[1]);
  radio.openReadingPipe(1, addresses[0]);

  if (DEBUG) {
    // For debugging info
    printf_begin();             // needed only once for printing details
    radio.printPrettyDetails(); // (larger) function that prints human readable data
  }
}

void loop() {
  long t = millis();
  byte data = 0x00;

  for (int i = 0; i < PEDAL_COUNT; i++) {
    data |= debounce(i) << PEDAL_COUNT - (i + 1);
  }

  // Ensure we have stopped listening (even if we're not) or we won't be able to transmit
  radio.stopListening(); 

  radio.write( &data, sizeof(byte) );

  if (KEEP_ALIVE_CIRCUIT_ENABLED) {
    if (isKeepAlive) {
      if (t - keepAliveTimer >= KEEP_ALIVE_ON_INTERVAL) {
        isKeepAlive = false;
        keepAliveTimer = t;
      }
    }
    else {
      if (t - keepAliveTimer >= KEEP_ALIVE_OFF_INTERVAL) {
        isKeepAlive = true;
        keepAliveTimer = t;
      }
    }
  }
  digitalWrite(keepAlivePin, isKeepAlive);

  delay(10);

  if (DEBUG) {
    // Show user what we sent and what we got back
    Serial.print("Sent: ");
    Serial.print(data);
    Serial.print(" Loop Time: ");
    Serial.println(millis() - t);
  }
}

boolean debounce(int thisPedal) {
 boolean current = digitalRead(pedalPin[thisPedal]);
 if (currentPedalVal[thisPedal] != current) {
   delay(5);
   current = digitalRead(pedalPin[thisPedal]);
 }
 currentPedalVal[thisPedal] = current;
 return !current;
}
