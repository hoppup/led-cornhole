#include "buttons.h"
#include "../include/config.h"

static unsigned long lastRedPress = 0;
static unsigned long lastBluePress = 0;
static unsigned long bothPressStart = 0;
static bool bothHeld = false;
static bool undoFired = false;
static bool resetFired = false;

void buttons_init() {
    pinMode(PIN_BTN_RED, INPUT_PULLUP);
    pinMode(PIN_BTN_BLUE, INPUT_PULLUP);
    
    Serial.println("[BUTTONS] Initialized (2 buttons: Red + Blue)");
    Serial.println("[BUTTONS] Both held 1s = Undo, Both held 3s = Reset");
}

ButtonEvent buttons_poll() {
    unsigned long now = millis();
    bool redDown = (digitalRead(PIN_BTN_RED) == LOW);
    bool blueDown = (digitalRead(PIN_BTN_BLUE) == LOW);
    
    // --- Both buttons held: Undo (1s) or Reset (3s) ---
    if (redDown && blueDown) {
        if (!bothHeld) {
            bothPressStart = now;
            bothHeld = true;
            undoFired = false;
            resetFired = false;
        }
        
        unsigned long holdTime = now - bothPressStart;
        
        // Reset at 3 seconds (fires once)
        if (!resetFired && holdTime >= RESET_HOLD_MS) {
            resetFired = true;
            // Also suppress undo re-fire and single presses on release
            lastRedPress = now;
            lastBluePress = now;
            Serial.println("[BUTTONS] RESET (both held 3s)");
            return BTN_RESET_HOLD;
        }
        
        // Undo at 1 second (fires once)
        if (!undoFired && holdTime >= UNDO_HOLD_MS) {
            undoFired = true;
            lastRedPress = now;
            lastBluePress = now;
            Serial.println("[BUTTONS] UNDO (both held 1s)");
            return BTN_UNDO;
        }
        
        return BTN_NONE;
    } else {
        bothHeld = false;
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
