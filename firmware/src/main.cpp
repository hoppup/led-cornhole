/**
 * LED Cornhole Scoreboard — Main Entry Point
 * 
 * ESP32-powered cornhole board with:
 * - WS2812B 7-segment LED scoreboard
 * - ESP-NOW board-to-board sync
 * - BLE mobile app connection
 * - LED effects (hole ring, celebrations)
 * - Buzzer sound effects
 * 
 * https://github.com/hoppup/led-cornhole
 */

#include <Arduino.h>
#include <FastLED.h>
#include "../include/config.h"

// TODO: Include module headers as they're developed
// #include "display.h"
// #include "espnow_comm.h"
// #include "ble_service.h"
// #include "buttons.h"
// #include "effects.h"
// #include "score.h"

// --- LED Array ---
CRGB leds[NUM_LEDS_TOTAL];

// --- Score State ---
uint8_t redScore = 0;
uint8_t blueScore = 0;
uint8_t gameRound = 0;

// --- Button State ---
unsigned long lastRedPress = 0;
unsigned long lastBluePress = 0;
unsigned long resetPressStart = 0;
bool resetHeld = false;

void setup() {
    Serial.begin(115200);
    Serial.println("=============================");
    Serial.println("  LED Cornhole Scoreboard");
    Serial.printf("  Board: %s\n", BLE_DEVICE_NAME);
    Serial.printf("  LEDs: %d total\n", NUM_LEDS_TOTAL);
    Serial.println("=============================");

    // --- Init FastLED ---
    FastLED.addLeds<WS2812B, PIN_LED_DATA, GRB>(leds, NUM_LEDS_TOTAL);
    FastLED.setBrightness(BRIGHTNESS_DEFAULT);
    FastLED.clear();
    FastLED.show();

    // --- Init Buttons ---
    pinMode(PIN_BTN_RED, INPUT_PULLUP);
    pinMode(PIN_BTN_BLUE, INPUT_PULLUP);
    pinMode(PIN_BTN_RESET, INPUT_PULLUP);

    // --- Init Buzzer ---
    pinMode(PIN_BUZZER, OUTPUT);
    digitalWrite(PIN_BUZZER, LOW);

    // --- TODO: Init ESP-NOW ---
    // espnow_init();

    // --- TODO: Init BLE ---
    // ble_init();

    // --- Startup Animation ---
    // TODO: Run LED test/chase animation
    Serial.println("Setup complete. Ready to play!");
}

void loop() {
    unsigned long now = millis();

    // --- Check Red Button ---
    if (digitalRead(PIN_BTN_RED) == LOW && (now - lastRedPress > DEBOUNCE_MS)) {
        lastRedPress = now;
        if (redScore < WINNING_SCORE) {
            redScore++;
            Serial.printf("Red: %d  Blue: %d\n", redScore, blueScore);
            // TODO: updateDisplay();
            // TODO: espnow_sendScore();
            // TODO: ble_notifyScore();
            // TODO: playScoreBeep();
        }
    }

    // --- Check Blue Button ---
    if (digitalRead(PIN_BTN_BLUE) == LOW && (now - lastBluePress > DEBOUNCE_MS)) {
        lastBluePress = now;
        if (blueScore < WINNING_SCORE) {
            blueScore++;
            Serial.printf("Red: %d  Blue: %d\n", redScore, blueScore);
            // TODO: updateDisplay();
            // TODO: espnow_sendScore();
            // TODO: ble_notifyScore();
            // TODO: playScoreBeep();
        }
    }

    // --- Check Reset Button (3-second hold) ---
    if (digitalRead(PIN_BTN_RESET) == LOW) {
        if (!resetHeld) {
            resetPressStart = now;
            resetHeld = true;
        } else if (now - resetPressStart >= RESET_HOLD_MS) {
            redScore = 0;
            blueScore = 0;
            gameRound = 0;
            resetHeld = false;
            Serial.println("GAME RESET!");
            // TODO: updateDisplay();
            // TODO: espnow_sendScore();
            // TODO: ble_notifyScore();
            // TODO: playResetSound();
        }
    } else {
        resetHeld = false;
    }

    // --- Check for win ---
    if (redScore >= WINNING_SCORE || blueScore >= WINNING_SCORE) {
        // TODO: playCelebration(redScore >= WINNING_SCORE ? COLOR_RED : COLOR_BLUE);
    }

    // --- TODO: Run ambient LED effects ---
    // effects_update();

    // Small delay to prevent tight loop
    delay(10);
}
