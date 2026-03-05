#ifndef EFFECTS_H
#define EFFECTS_H

#include <FastLED.h>
#include "score.h"

// Initialize effects (hole ring, ambient)
void effects_init(CRGB* leds);

// Call every loop iteration — runs ambient animations
void effects_update(CRGB* leds, const GameState &state);

// Trigger a win celebration (blocking animation)
void effects_winCelebration(CRGB* leds, bool redWon);

// Trigger a score flash on the hole ring
void effects_scoreFlash(CRGB* leds, bool isRed);

// Reset animation (quick wipe)
void effects_resetAnimation(CRGB* leds);

#endif
