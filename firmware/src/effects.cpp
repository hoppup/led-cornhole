#include "effects.h"
#include "../include/config.h"

static unsigned long lastHoleUpdate = 0;
static uint8_t holeHue = 0;

void effects_init(CRGB* leds) {
    // Set hole ring to idle state — slow rainbow
    for (int i = 0; i < LEDS_HOLE_RING; i++) {
        leds[IDX_HOLE_RING + i] = CHSV(i * (256 / LEDS_HOLE_RING), 255, 80);
    }
    FastLED.show();
    Serial.println("[EFFECTS] Initialized");
}

void effects_update(CRGB* leds, const GameState &state) {
    unsigned long now = millis();
    
    // --- Hole ring: slow rainbow rotation (update every 50ms) ---
    if (now - lastHoleUpdate > 50) {
        lastHoleUpdate = now;
        holeHue++;
        
        if (state.gameOver) {
            // During win state — fast pulse in winner's color
            CRGB winColor = (state.redScore >= WINNING_SCORE) ? COLOR_RED : COLOR_BLUE;
            uint8_t brightness = beatsin8(120, 50, 255); // Fast pulse
            for (int i = 0; i < LEDS_HOLE_RING; i++) {
                leds[IDX_HOLE_RING + i] = winColor;
                leds[IDX_HOLE_RING + i].nscale8(brightness);
            }
        } else if (state.redScore == 0 && state.blueScore == 0) {
            // Idle — slow rainbow
            for (int i = 0; i < LEDS_HOLE_RING; i++) {
                leds[IDX_HOLE_RING + i] = CHSV(holeHue + i * (256 / LEDS_HOLE_RING), 255, 80);
            }
        } else {
            // Active game — split red/blue with gentle pulse
            uint8_t brightness = beatsin8(30, 60, 150);
            int half = LEDS_HOLE_RING / 2;
            for (int i = 0; i < half; i++) {
                leds[IDX_HOLE_RING + i] = COLOR_RED;
                leds[IDX_HOLE_RING + i].nscale8(brightness);
            }
            for (int i = half; i < LEDS_HOLE_RING; i++) {
                leds[IDX_HOLE_RING + i] = COLOR_BLUE;
                leds[IDX_HOLE_RING + i].nscale8(brightness);
            }
        }
    }
    
    // Edge LEDs would go here when installed
    // if (LEDS_EDGE > 0) { ... }
}

void effects_scoreFlash(CRGB* leds, bool isRed) {
    // Flash the hole ring in the scoring team's color
    CRGB color = isRed ? COLOR_RED : COLOR_BLUE;
    
    for (int flash = 0; flash < 3; flash++) {
        for (int i = 0; i < LEDS_HOLE_RING; i++) {
            leds[IDX_HOLE_RING + i] = color;
        }
        FastLED.show();
        delay(100);
        
        for (int i = 0; i < LEDS_HOLE_RING; i++) {
            leds[IDX_HOLE_RING + i] = COLOR_OFF;
        }
        FastLED.show();
        delay(100);
    }
}

void effects_winCelebration(CRGB* leds, bool redWon) {
    Serial.printf("[EFFECTS] WIN CELEBRATION — %s wins!\n", redWon ? "RED" : "BLUE");
    
    CRGB winColor = redWon ? COLOR_RED : COLOR_BLUE;
    unsigned long start = millis();
    
    while (millis() - start < WIN_ANIM_DURATION) {
        // Rainbow chase across ALL LEDs
        uint8_t hue = (millis() / 10) % 256;
        for (int i = 0; i < NUM_LEDS_TOTAL; i++) {
            leds[i] = CHSV(hue + i * 4, 255, 200);
        }
        FastLED.show();
        delay(20);
        
        // Alternate with winner's color flash every 500ms
        if (((millis() - start) / 500) % 2 == 0) {
            for (int i = 0; i < LEDS_SCOREBOARD; i++) {
                leds[i] = winColor;
            }
            FastLED.show();
            delay(20);
        }
    }
}

void effects_resetAnimation(CRGB* leds) {
    Serial.println("[EFFECTS] Reset animation");
    
    // Quick wipe from left to right — green sweep
    for (int i = 0; i < NUM_LEDS_TOTAL; i++) {
        leds[i] = COLOR_GREEN;
        if (i >= 3) leds[i-3] = COLOR_OFF;
        FastLED.show();
        delay(3);
    }
    
    // Clear everything
    fill_solid(leds, NUM_LEDS_TOTAL, COLOR_OFF);
    FastLED.show();
    delay(200);
}
