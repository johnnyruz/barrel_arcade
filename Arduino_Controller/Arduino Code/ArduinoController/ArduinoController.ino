#include "Arduino.h"
#include <SPI.h>
#include <RF24.h>
#include <EEPROM.h>
#include "printf.h"
#include "Mouse.h"
#include "Keyboard.h"

#define RIGHT_FLIPPER_IN 2
#define LEFT_FLIPPER_IN 3

#define RIGHT_FLIPPER_OUT 8
#define LEFT_FLIPPER_OUT 9

#define LED_OUT 10

#define CE_PIN A1
#define CS_PIN A0
#define RADIO_STATUS_LED 7

#define DEBUG false

#define PEDAL_COUNT 3

bool flipper_enabled = false;
bool led_enabled = true;

bool currentPedalVal[PEDAL_COUNT];
char pedalFunction[PEDAL_COUNT];
bool pedalStatus[PEDAL_COUNT];

RF24 radio(CE_PIN, CS_PIN);
byte addresses[][6] = {"1Node", "2Node"};
long lastRadioRead = 0;

// the setup function runs once when you press reset or power the board
void setup() {
  // initialize digital pin LED_BUILTIN as an output.
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);

  pinMode(LEFT_FLIPPER_IN, INPUT_PULLUP);
  pinMode(RIGHT_FLIPPER_IN, INPUT_PULLUP);
  
  pinMode(LEFT_FLIPPER_OUT, OUTPUT);
  pinMode(RIGHT_FLIPPER_OUT, OUTPUT);
  digitalWrite(RIGHT_FLIPPER_OUT, HIGH);
  digitalWrite(LEFT_FLIPPER_OUT, HIGH);
  
  pinMode(LED_OUT, OUTPUT);
  pinMode(RADIO_STATUS_LED, OUTPUT);
  
  loadPedalBindings();
  
  Serial.begin(115200);

  //CONFIGURE PEDAL RECEIVER
    // Initiate the radio object
  radio.begin();
  // Set the transmit power to lowest available to prevent power supply related issues
  radio.setPALevel(RF24_PA_MIN);
  // Set the speed of the transmission to the quickest available
  radio.setDataRate(RF24_2MBPS);
  // Use a channel unlikely to be used by Wifi, Microwave ovens etc
  radio.setChannel(124);

  // Open a writing and reading pipe on each radio, with opposite addresses
  radio.openWritingPipe(addresses[0]);
  radio.openReadingPipe(1, addresses[1]);
  
  if (DEBUG) {
    // For debugging info
    printf_begin();             // needed only once for printing details
    radio.printPrettyDetails(); // (larger) function that prints human readable data
  }
}

// the loop function runs over and over again forever
void loop() {

  // FLIPPER LOGIC
  if (flipper_enabled) {
    digitalWrite(LEFT_FLIPPER_OUT, digitalRead(LEFT_FLIPPER_IN));
    digitalWrite(RIGHT_FLIPPER_OUT, digitalRead(RIGHT_FLIPPER_IN));
  }
  else {
    digitalWrite(LEFT_FLIPPER_OUT, HIGH);
    digitalWrite(RIGHT_FLIPPER_OUT, HIGH);
  }

  // START & COIN BUTTON LED LOGIC
  digitalWrite(LED_OUT, led_enabled);

  // WIRELESS PEDAL FUNCTIONALITY
  byte data = 0x00;
  radio.startListening();
    
  if ( radio.available()) {
    // Go and read the data and put it into that variable
    while (radio.available()) {
      radio.read( &data, sizeof(byte));
      lastRadioRead = millis();
    }

    if (DEBUG) {
      Serial.print("Received Value: ");
      Serial.println(data);
    }

    processPedalInput(data);
  }

  // WRITE RADIO STATUS LED
  digitalWrite(RADIO_STATUS_LED, millis() - lastRadioRead <= 1000);


  // CONFIGURATION LOGIC
  if (Serial.available()) {
    char c = Serial.read();

    //CONFIGURE PEDAL FUNCTIONS
    if (c == '#') {
      int i = PEDAL_COUNT - 1;
      while (Serial.available()) {
        c = Serial.read();
        if (i >= 0) {
          pedalFunction[i] = c;
          EEPROM.update(i, c);
          Serial.print("Setting Button To: ");
          Serial.println(c);
        }
        i--;
      }
    }
    // Toggle Flipper Function
    else if (c == 'f') {
      flipper_enabled = !flipper_enabled;
      digitalWrite(LED_BUILTIN,flipper_enabled);
      ack_flipper_cmd(flipper_enabled);
    }
    // Toggle Button LEDs
    else if (c == 'l') {
      led_enabled = !led_enabled;
    }
  }

  delay(10);

}

void ack_flipper_cmd(bool isOn) {
  digitalWrite(LED_OUT, !led_enabled);
  digitalWrite(LEFT_FLIPPER_OUT, !isOn);
  digitalWrite(RIGHT_FLIPPER_OUT, !isOn);
  delay(200);
  digitalWrite(LED_OUT, led_enabled);
  digitalWrite(LEFT_FLIPPER_OUT, HIGH);
  digitalWrite(RIGHT_FLIPPER_OUT, HIGH);  
}

void processPedalInput(byte data) {

  for (int i = PEDAL_COUNT - 1; i >= 0; i--) {
    bool isOn = bitRead(data, i);

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

void loadPedalBindings() {

  for (int i = PEDAL_COUNT-1; i >= 0; i--) {
    char c = EEPROM.read(i);
    if (byte(c) == byte(0xFF)) { //EEPROM never written, default to right mouse button
      EEPROM.write(0, '@');
      c = '@';
    }
    pedalFunction[i] = c;
  }
}
