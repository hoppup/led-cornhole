#include "buttons.h"
#include "../include/config.h"

static unsigned long lastRedPress = 0;
static unsigned long lastBluePress = 0;
static unsigned long resetPressStart = 0;
static unsigned long bothPressStart = 0;
static bool resetHeld = false;
static bool bothHeld = false;
static bool undoFired = false;

void buttons_init() {
    pinMode(PIN_BTN_RED, INPUT_PULLUP);
    pinMode(PIN_BTN_BLUE, INPUT_PULLUP);
    pinMode(PIN_BTN_RESET, INPUT_PULLUP);
    
    Serial.println("[BUTTONS] Initialized (INPUT_PULLUP, active LOW)");
}

ButtonEvent buttons_poll() {
    unsigned long now = millis();
    bool redDown = (digitalRead(PIN_BTN_RED) == LOW);
    bool blueDown = (digitalRead(PIN_BTN_BLUE) == LOW);
    bool resetDown = (digitalRead(PIN_BTN_RESET) == LOW);
    
    // --- Undo: both red + blue held for UNDO_HOLD_MS ---
    if (redDown && blueDown) {
        if (!bothHeld) {
            bothPressStart = now;
            bothHeld = true;
            undoFired = false;
        } else if (!undoFired && (now - bothPressStart >= UNDO_HOLD_MS)) {
            undoFired = true;
            // Reset individual press tracking so releasing doesn't trigger single presses
            lastRedPress = now;
            lastBluePress = now;
            Serial.println("[BUTTONS] UNDO (both held)");
            return BTN_UNDO;
        }
        return BTN_NONE;
    } else {
        bothHeld = false;
    }
    
    // --- Reset: held for RESET_HOLD_MS ---
    if (resetDown) {
        if (!resetHeld) {
            resetPressStart = now;
            resetHeld = true;
        } else if (now - resetPressStart >= RESET_HOLD_MS) {
            resetHeld = false; // Only fire once per hold
            Serial.println("[BUTTONS] RESET (held 3s)");
            return BTN_RESET_HOLD;
        }
        return BTN_NONE;
    } else {
        resetHeld = false;
    }
    
    // --- Red button: single press with debounce ---
    if (redDown && !blueDown && (now - lastRedPress > DEBOUNCE_MS)) {
        lastRedPress = now;
        Serial.println("[BUTTONS] Red pressed");
        return BTN_RED_PRESS;
    }
    
    // --- Blue button: single press with debounce ---
    if (blueDown && !redDown && (now - lastBluePress > DEBOUNCE_MS)) {
        lastBluePress = now;
        Serial.println("[BUTTONS] Blue pressed");
        return BTN_BLUE_PRESS;
    }
    
    return BTN_NONE;
}
