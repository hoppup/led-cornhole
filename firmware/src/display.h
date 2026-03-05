#ifndef DISPLAY_H
#define DISPLAY_H

#include <FastLED.h>
#include "score.h"

// Initialize the LED display
void display_init(CRGB* leds);

// Update the full scoreboard display from game state
void display_updateScore(CRGB* leds, const GameState &state);

// Flash a team's digits briefly (called on score change)
void display_flashScore(CRGB* leds, bool isRed);

// Show a single digit value (0-9) at a digit position (0-3) in given color
void display_setDigit(CRGB* leds, uint8_t position, uint8_t value, CRGB color);

// Set the separator dots
void display_setSeparator(CRGB* leds, CRGB color);

// Clear the entire scoreboard zone
void display_clearScoreboard(CRGB* leds);

// Startup animation (LED test)
void display_startupAnimation(CRGB* leds);

#endif
