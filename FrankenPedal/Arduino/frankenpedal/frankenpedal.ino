#include "Mouse.h"
#include "Keyboard.h"
#include <EEPROM.h>

const int pedalLeftPin = 2;
const int pedalMiddlePin = A3;
const int pedalRightPin = 3;
const int pedalPin [] = {pedalLeftPin, pedalMiddlePin, pedalRightPin};

boolean currentPedalVal[3];
char pedalFunction [] = {'~','!','@'};

bool leftIsPressed = false;
bool rightIsPressed = false;
bool middleIsPressed = false;

char currentlyMapping = '-';

void setup() {
  pinMode(pedalLeftPin, INPUT_PULLUP);
  pinMode(pedalMiddlePin, INPUT_PULLUP);
  pinMode(pedalRightPin, INPUT_PULLUP);

  loadBindings();

  Serial.begin(115200);  
}

void loop() {

  // BUTTON PRESS LOGIC!
  if (debounce(0)) // Left Pedal is physically pressed
  {
    if (!leftIsPressed) // If we have not already sent the DOWN event for this press 
    {
      //Serial.println("PRESS_LEFT");
      leftIsPressed = true;
      pressPedal(0);
    }
  }
  else if (leftIsPressed) // Left Pedal is physically released but we have not yet sent the UP event
  { 
    //Serial.println("RELEASE_LEFT");
    leftIsPressed = false;
    releasePedal(0);
  }
    
  if (debounce(1)) // Middle Pedal is physically pressed
  {
    if (!middleIsPressed) // If we have not already sent the DOWN event for this press
    {
      //Serial.println("PRESS_MIDDLE");
      middleIsPressed = true;
      pressPedal(1);
    }
  }
  else if (middleIsPressed) // Middle Pedal is physically released but we have not yet sent the UP event
  {
    //Serial.println("RELEASE_MIDDLE");
    middleIsPressed = false;
    releasePedal(1);
  }

  if (debounce(2)) // Right Pedal is physically pressed
  {
    if (!rightIsPressed) // If we have not already sent the DOWN event for this press
    {
      //Serial.println("PRESS_RIGHT");
      rightIsPressed = true;
      pressPedal(2);
    }
  }
  else if (rightIsPressed) // Right Pedal is physically released but we have not yet sent the UP event
  { 
    //Serial.println("RELEASE_RIGHT");
    rightIsPressed = false;
    releasePedal(2);
  }

  // PEDAL CONFIGURATION LOGIC
  if (Serial.available()) {
      char c = Serial.read(); // Reads single character from serial input
      if (currentlyMapping == '-' && ( c == '1' || c == '2' || c == '3' )) {
        currentlyMapping = c;      
      }
      else {
        switch (currentlyMapping) {
          case '1':
            pedalFunction[0] = c;
            EEPROM.update(0, c);
            break;
          case '2':
            pedalFunction[1] = c;
            EEPROM.update(1, c);
            break;
          case '3':
            pedalFunction[2] = c;
            EEPROM.update(2, c);
            break;
          default:
            break;          
        }
        currentlyMapping = '-';
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
  char left = EEPROM.read(0);
  char middle = EEPROM.read(1);
  char right = EEPROM.read(2);

  if (byte(left) == byte(0xFF)) { // EEPROM never written, default to left mouse button
    left = '~';
    EEPROM.write(0, left);
  }

  if (byte(middle) == byte(0xFF)) { // EEPROM never written, default to middle mouse button
    middle = '!';
    EEPROM.write(1, middle);
  }

  if (byte(right) == byte(0xFF)) { // EEPROM never written, default to right mouse button
    right = '@';
    EEPROM.write(2, right);
  }

  pedalFunction[0] = left;
  pedalFunction[1] = middle;
  pedalFunction[2] = right;
}
