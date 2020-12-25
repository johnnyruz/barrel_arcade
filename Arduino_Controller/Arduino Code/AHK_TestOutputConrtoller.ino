#define LEFT_FLIPPER_IN A1
#define RIGHT_FLIPPER_IN A0

#define LEFT_FLIPPER_OUT 2
#define RIGHT_FLIPPER_OUT 3
#define LED_OUT 4

bool flipper_enabled = false;
bool led_enabled = true;

// the setup function runs once when you press reset or power the board
void setup() {
  // initialize digital pin LED_BUILTIN as an output.
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);

  pinMode(LEFT_FLIPPER_IN, INPUT_PULLUP);
  pinMode(RIGHT_FLIPPER_IN, INPUT_PULLUP);
  pinMode(LEFT_FLIPPER_OUT, OUTPUT);
  pinMode(RIGHT_FLIPPER_OUT, OUTPUT);
  pinMode(LED_OUT, OUTPUT);
  
  Serial.begin(115200);
}

// the loop function runs over and over again forever
void loop() {

  if (flipper_enabled) {
    digitalWrite(LEFT_FLIPPER_OUT, digitalRead(LEFT_FLIPPER_IN));
    digitalWrite(RIGHT_FLIPPER_OUT, digitalRead(RIGHT_FLIPPER_IN));
  }
  else {
    digitalWrite(LEFT_FLIPPER_OUT, HIGH);
    digitalWrite(RIGHT_FLIPPER_OUT, HIGH);
  }

  digitalWrite(LED_OUT, led_enabled);

  if (Serial.available()) {
    char c = Serial.read();

    // Toggle Flipper Function
    if (c == 'f') {
      flipper_enabled = !flipper_enabled;
      digitalWrite(LED_BUILTIN,flipper_enabled);
      ack_flipper_cmd(flipper_enabled);
    }

    // Toggle Button LEDs
    if (c == 'l') {
      led_enabled = !led_enabled;
    }

    // Experiment!
    if (c == 'e') {
      if (flipper_enabled) {
        digitalWrite(LEFT_FLIPPER_OUT, LOW);
        delay(100);
      }
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
