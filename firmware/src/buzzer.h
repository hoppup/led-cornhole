#ifndef BUZZER_H
#define BUZZER_H

#include <Arduino.h>

void buzzer_init();
void buzzer_scoreBeep();       // Quick beep on score
void buzzer_undoBeep();        // Double beep on undo
void buzzer_resetSound();      // Descending tone on reset
void buzzer_winFanfare();      // Victory melody
void buzzer_startupSound();    // Boot chime

#endif
