#include "esp32-hal.h"
#include "esp_log_level.h"
#include <Arduino.h>
#include <climits>

// Check the door switches on the following pins.
const uint8_t CHECK_PINS[] = {22, 23};
// If any door becomes open, buzz the buzzer on the following pin.
const uint8_t BUZZER_PIN = 21;
// Buzz the buzzer every X milliseconds,
const int BUZZER_EVERY_MS = 30000; // 30 seconds
// at this frequency,
const int BUZZER_FREQ = 440; // A4
// this many times.
const uint8_t BUZZ_TIMES = 3; // buzz 3 times per buzz
// After repeating the sequence this many times, buzz continuously.
const uint MAX_BUZZER_ITERATIONS = 5;
//
// The duration of each individual buzz.
const uint BUZZER_DURATION_MS = 500;
const char *TAG = "Buzzer";

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

  for (uint8_t pin : CHECK_PINS) {
    pinMode(pin, INPUT_PULLUP);
  }

  esp_log_level_set(TAG, ESP_LOG_INFO);
  ESP_LOGI(TAG, "Setup complete");
}

void loop() {
  bool open = false;
  for (uint8_t pin : CHECK_PINS) {
    if (digitalRead(pin) == HIGH) {
      open = true;
      ESP_LOGV("Door", "Door open");
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

  delay(100);
}