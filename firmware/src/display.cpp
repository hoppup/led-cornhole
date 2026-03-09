#include "display.h"
#include "../include/config.h"

// ============================================================
// 7-Segment Display — Maps digits 0-9 to segment patterns
// ============================================================
//
// Segment layout per digit:
//
//    aaaa
//   f    b
//   f    b
//    gggg
//   e    c
//   e    c
//    dddd
//
// Segments are wired in order: a, b, c, d, e, f, g
// Each segment has LEDS_PER_SEGMENT LEDs (4)
//
// 7-segment encoding: bit0=a, bit1=b, bit2=c, bit3=d, bit4=e, bit5=f, bit6=g
// ============================================================

static const uint8_t SEGMENT_MAP[10] = {
    // abcdefg
    0b1110111, // 0: a,b,c,d,e,f    (all except g)
    0b0100100, // 1: b,c
    0b1101011, // 2: a,b,d,e,g
    0b1101101, // 3: a,b,c,d,g
    0b0101100, // 4: b,c,f,g        — wait, let me recalculate
    0b1001101, // 5: a,c,d,f,g
    0b1011111, // 6: a,c,d,e,f,g
    0b1100100, // 7: a,b,c
    0b1111111, // 8: all
    0b1101101, // 9: a,b,c,d,f,g
};

// Correct 7-segment encoding with proper bit positions
// bit6=a(top), bit5=b(upper-right), bit4=c(lower-right), 
// bit3=d(bottom), bit2=e(lower-left), bit1=f(upper-left), bit0=g(middle)
static const uint8_t DIGIT_SEGMENTS[10] = {
    0b1111110, // 0: a,b,c,d,e,f
    0b0110000, // 1: b,c
    0b1101101, // 2: a,b,d,e,g
    0b1111001, // 3: a,b,c,d,g
    0b0110011, // 4: b,c,f,g
    0b1011011, // 5: a,c,d,f,g
    0b1011111, // 6: a,c,d,e,f,g
    0b1110000, // 7: a,b,c
    0b1111111, // 8: a,b,c,d,e,f,g
    0b1111011, // 9: a,b,c,d,f,g
};

// Get the starting LED index for a given digit position (0-3)
static uint16_t getDigitStartIndex(uint8_t position) {
    switch (position) {
        case 0: return IDX_DIGIT_0;  // Red tens
        case 1: return IDX_DIGIT_1;  // Red ones
        case 2: return IDX_DIGIT_2;  // Blue tens
        case 3: return IDX_DIGIT_3;  // Blue ones
        default: return 0;
    }
}

void display_init(CRGB* leds) {
    // Clear all LEDs
    fill_solid(leds, NUM_LEDS_TOTAL, COLOR_OFF);
    FastLED.show();
}

void display_setDigit(CRGB* leds, uint8_t position, uint8_t value, CRGB color) {
    if (value > 9 || position > 3) return;
    
    uint16_t startIdx = getDigitStartIndex(position);
    uint8_t segments = DIGIT_SEGMENTS[value];
    
    // Walk through 7 segments (a=bit6 down to g=bit0)
    for (int seg = 0; seg < 7; seg++) {
        bool on = (segments >> (6 - seg)) & 0x01;
        CRGB segColor = on ? color : COLOR_OFF;
        
        // Each segment is LEDS_PER_SEGMENT consecutive LEDs
        uint16_t segStart = startIdx + (seg * LEDS_PER_SEGMENT);
        for (int led = 0; led < LEDS_PER_SEGMENT; led++) {
            leds[segStart + led] = segColor;
        }
    }
}

void display_setSeparator(CRGB* leds, CRGB color) {
    // Separator disabled for testing
    #if LEDS_SEPARATOR > 0
    for (int i = 0; i < LEDS_SEPARATOR; i++) {
        leds[IDX_SEPARATOR + i] = color;
    }
    #endif
}

void display_clearScoreboard(CRGB* leds) {
    for (int i = 0; i < LEDS_SCOREBOARD; i++) {
        leds[i] = COLOR_OFF;
    }
}

void display_updateScore(CRGB* leds, const GameState &state) {
    // Red score — digits 0 and 1
    uint8_t redTens = state.redScore / 10;
    uint8_t redOnes = state.redScore % 10;
    
    // Blue score — digits 2 and 3
    uint8_t blueTens = state.blueScore / 10;
    uint8_t blueOnes = state.blueScore % 10;
    
    // Set red digits
    if (redTens > 0) {
        display_setDigit(leds, 0, redTens, COLOR_RED);
    } else {
        // Blank leading zero — turn off all segments
        uint16_t startIdx = getDigitStartIndex(0);
        for (int i = 0; i < LEDS_PER_DIGIT; i++) {
            leds[startIdx + i] = COLOR_OFF;
        }
    }
    display_setDigit(leds, 1, redOnes, COLOR_RED);
    
    // Set separator dots (white)
    display_setSeparator(leds, COLOR_WHITE);
    
    // Set blue digits
    if (blueTens > 0) {
        display_setDigit(leds, 2, blueTens, COLOR_BLUE);
    } else {
        uint16_t startIdx = getDigitStartIndex(2);
        for (int i = 0; i < LEDS_PER_DIGIT; i++) {
            leds[startIdx + i] = COLOR_OFF;
        }
    }
    display_setDigit(leds, 3, blueOnes, COLOR_BLUE);
    
    FastLED.show();
}

void display_flashScore(CRGB* leds, bool isRed) {
    // Quick bright flash on the scoring team's digits
    CRGB flashColor = isRed ? CRGB(255, 255, 255) : CRGB(255, 255, 255);
    uint8_t d0 = isRed ? 0 : 2;
    uint8_t d1 = isRed ? 1 : 3;
    
    // Flash white
    uint16_t start0 = getDigitStartIndex(d0);
    uint16_t start1 = getDigitStartIndex(d1);
    for (int i = 0; i < LEDS_PER_DIGIT; i++) {
        leds[start0 + i] = flashColor;
        leds[start1 + i] = flashColor;
    }
    FastLED.show();
    delay(SCORE_FLASH_MS);
}

void display_startupAnimation(CRGB* leds) {
    Serial.println("[DISPLAY] Running startup animation...");
    
    // Chase through all LEDs
    for (int i = 0; i < NUM_LEDS_TOTAL; i++) {
        leds[i] = CRGB(0, 255, 0);
        if (i > 0) leds[i-1] = COLOR_OFF;
        FastLED.show();
        delay(5);
    }
    leds[NUM_LEDS_TOTAL - 1] = COLOR_OFF;
    
    // Flash all segments on (test pattern — "88:88")
    for (int d = 0; d < 4; d++) {
        CRGB color = (d < 2) ? COLOR_RED : COLOR_BLUE;
        display_setDigit(leds, d, 8, color);
    }
    display_setSeparator(leds, COLOR_WHITE);
    FastLED.show();
    delay(1000);
    
    // Clear and show 0:0
    display_clearScoreboard(leds);
    FastLED.show();
    delay(200);
    
    Serial.println("[DISPLAY] Startup complete.");
}
