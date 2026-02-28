#include "esp32-hal.h"
#include <Arduino.h>
#include <climits>

const uint8_t CHECK_PINS[] = {23};
const uint8_t BUZZER_PIN = 21;
const int BUZZER_EVERY_MS = 30000;    // 30 seconds
const int BUZZER_FREQ = 440;          // A4
const uint8_t BUZZ_TIMES = 3;         // buzz 3 times per buzz
const uint MAX_BUZZER_ITERATIONS = 5; // after this many times, buzz forever
const uint BUZZER_DURATION_MS = 500;  // duration of each buzz

const int CLOSED = -1;
int lastOpenTime = CLOSED;
uint buzzerIterations = 0;

void resetBuzzer() {
  lastOpenTime = CLOSED;
  buzzerIterations = 0;
}

void buzz() {
  for (uint8_t i = 0; i < BUZZ_TIMES; ++i) {
    tone(BUZZER_PIN, BUZZER_FREQ, 0);
    delay(BUZZER_DURATION_MS);
    noTone(BUZZER_PIN);
    delay(BUZZER_DURATION_MS);
  }
}

void setup() {
  Serial.begin(115200);
  Serial.println("Hello, World!");

  for (uint8_t pin : CHECK_PINS) {
    pinMode(pin, INPUT_PULLUP);
  }
}

void loop() {
  bool open = false;
  for (uint8_t pin : CHECK_PINS) {
    if (digitalRead(pin) == HIGH) {
      open = true;
    }
  }

  if (open) {
    if (lastOpenTime == CLOSED) {
      lastOpenTime = millis();
    } else if (buzzerIterations > MAX_BUZZER_ITERATIONS ||
               millis() - lastOpenTime > BUZZER_EVERY_MS) {
      buzz();
      buzzerIterations++;
      lastOpenTime = millis();
    }
  } else {
    resetBuzzer();
  }
}