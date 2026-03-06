#include "buzzer.h"
#include "../include/config.h"

// Use LEDC for tone generation on ESP32
// (Arduino tone() doesn't work on ESP32 — use ledcWriteTone instead)

#define BUZZER_CHANNEL  0
#define BUZZER_RESOLUTION 8

static void playTone(uint16_t freq, uint16_t duration) {
    ledcWriteTone(BUZZER_CHANNEL, freq);
    delay(duration);
    ledcWriteTone(BUZZER_CHANNEL, 0);
}

void buzzer_init() {
    // ESP32-S3 uses newer ledcAttach(), standard ESP32 uses ledcSetup() + ledcAttachPin()
    #ifdef CONFIG_IDF_TARGET_ESP32S3
        ledcAttach(PIN_BUZZER, 2000, BUZZER_RESOLUTION);
    #else
        ledcSetup(BUZZER_CHANNEL, 2000, BUZZER_RESOLUTION);
        ledcAttachPin(PIN_BUZZER, BUZZER_CHANNEL);
    #endif
    ledcWriteTone(BUZZER_CHANNEL, 0); // Start silent
    Serial.println("[BUZZER] Initialized");
}

void buzzer_scoreBeep() {
    // Quick high beep
    playTone(1500, 80);
}

void buzzer_undoBeep() {
    // Two quick low beeps
    playTone(800, 80);
    delay(60);
    playTone(600, 80);
}

void buzzer_resetSound() {
    // Descending three-note
    playTone(1000, 100);
    delay(50);
    playTone(700, 100);
    delay(50);
    playTone(400, 200);
}

void buzzer_winFanfare() {
    // Victory melody — charge!
    playTone(523, 150);  // C
    delay(30);
    playTone(659, 150);  // E
    delay(30);
    playTone(784, 150);  // G
    delay(30);
    playTone(1047, 300); // High C
    delay(100);
    playTone(784, 100);  // G
    playTone(1047, 400); // High C (hold)
}

void buzzer_startupSound() {
    // Quick ascending boot chime
    playTone(880, 80);   // A
    delay(30);
    playTone(1175, 80);  // D
    delay(30);
    playTone(1760, 120); // A (high)
}
