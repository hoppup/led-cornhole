#ifndef SCORE_H
#define SCORE_H

#include <Arduino.h>

// Score state — shared across modules
struct GameState {
    uint8_t redScore;
    uint8_t blueScore;
    uint8_t round;
    bool    gameOver;
    
    // Undo history (last action)
    uint8_t lastRedScore;
    uint8_t lastBlueScore;
    bool    canUndo;
};

void score_init(GameState &state);
void score_addRed(GameState &state);
void score_addBlue(GameState &state);
void score_undo(GameState &state);
void score_reset(GameState &state);
bool score_checkWin(const GameState &state);

#endif
