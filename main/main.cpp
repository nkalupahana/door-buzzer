#include <Arduino.h>

const uint8_t CHECK_PINS[] = {23};
const uint8_t BUZZER_PIN = 14;

void setup() {
  Serial.begin(115200);
  Serial.println("Hello, World!");

  for (uint8_t pin : CHECK_PINS) {
    pinMode(pin, INPUT_PULLUP);
  }
  pinMode(BUZZER_PIN, OUTPUT);
}

void loop() {
  bool open = false;
  for (uint8_t pin : CHECK_PINS) {
    if (digitalRead(pin) == LOW) {
      open = true;
    }
  }

  Serial.println(open);
}