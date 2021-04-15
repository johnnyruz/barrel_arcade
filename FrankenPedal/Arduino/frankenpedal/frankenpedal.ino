#include "Mouse.h"
#include "Keyboard.h"
#include <EEPROM.h>

const int pedalLeftPin = 2;
const int pedalMiddlePin = 3;
const int pedalRightPin = 4;
const int pedalPin [] = {pedalLeftPin, pedalMiddlePin, pedalRightPin};

const int PEDAL_COUNT = ( sizeof(pedalPin) / sizeof(int) );

bool currentPedalVal[PEDAL_COUNT];
char pedalFunction[PEDAL_COUNT];
bool pedalStatus[PEDAL_COUNT];

void setup() {
  for (int i = 0; i < PEDAL_COUNT; i++) {
    pinMode(pedalPin[i], INPUT_PULLUP);
  }
  loadBindings();

  Serial.begin(115200);  
}

void loop() {

  for (int i = 0; i < PEDAL_COUNT; i++) {
    bool isOn = debounce(i);
    if (isOn) {
      if (!pedalStatus[i]) {
        pedalStatus[i] = true;
        pressPedal(i);
      }
    }
    else if (pedalStatus[i]) {
      pedalStatus[i] = false;
      releasePedal(i);
    }
  }

  // PEDAL CONFIGURATION LOGIC
  // CONFIGURATION LOGIC
  if (Serial.available()) {
    char c = Serial.read();

    //CONFIGURE PEDAL FUNCTIONS
    if (c == '#') {
      int i = 0;
      while (Serial.available()) {
        c = Serial.read();
        if (i < PEDAL_COUNT) {
          pedalFunction[i] = c;
          EEPROM.update(i, c);
          Serial.print("Setting Button To: ");
          Serial.println(c);
        }
        i++;
      }
    }
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

void pressPedal(int pedal) {
  switch (pedalFunction[pedal]) {
    case '~':
      Mouse.press(MOUSE_LEFT);
      break;
    case '!':
      Mouse.press(MOUSE_MIDDLE);
      break;
    case '@':
      Mouse.press(MOUSE_RIGHT);
      break;
    default:
      Keyboard.press(pedalFunction[pedal]);
      break;
  }
}

void releasePedal(int pedal) {
  switch (pedalFunction[pedal]) {
    case '~':
      Mouse.release(MOUSE_LEFT);
      break;
    case '!':
      Mouse.release(MOUSE_MIDDLE);
      break;
    case '@':
      Mouse.release(MOUSE_RIGHT);
      break;
    default:
      Keyboard.release(pedalFunction[pedal]);
      break;
  }
}

void loadBindings() {
  for (int i = 0; i < PEDAL_COUNT; i++) {
    char c = EEPROM.read(i);
    if (byte(c) == byte(0xFF)) { //EEPROM never written, default to right mouse button
      EEPROM.write(i, '@');
      c = '@';
    }
    pedalFunction[i] = c;
  }
}
