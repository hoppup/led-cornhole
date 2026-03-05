#include "score.h"
#include "../include/config.h"

void score_init(GameState &state) {
    state.redScore = 0;
    state.blueScore = 0;
    state.round = 0;
    state.gameOver = false;
    state.lastRedScore = 0;
    state.lastBlueScore = 0;
    state.canUndo = false;
}

void score_addRed(GameState &state) {
    if (state.gameOver || state.redScore >= WINNING_SCORE) return;
    
    // Save for undo
    state.lastRedScore = state.redScore;
    state.lastBlueScore = state.blueScore;
    state.canUndo = true;
    
    state.redScore++;
    
    if (state.redScore >= WINNING_SCORE) {
        state.gameOver = true;
    }
    
    Serial.printf("[SCORE] Red: %d  Blue: %d\n", state.redScore, state.blueScore);
}

void score_addBlue(GameState &state) {
    if (state.gameOver || state.blueScore >= WINNING_SCORE) return;
    
    // Save for undo
    state.lastRedScore = state.redScore;
    state.lastBlueScore = state.blueScore;
    state.canUndo = true;
    
    state.blueScore++;
    
    if (state.blueScore >= WINNING_SCORE) {
        state.gameOver = true;
    }
    
    Serial.printf("[SCORE] Red: %d  Blue: %d\n", state.redScore, state.blueScore);
}

void score_undo(GameState &state) {
    if (!state.canUndo) return;
    
    state.redScore = state.lastRedScore;
    state.blueScore = state.lastBlueScore;
    state.gameOver = false;
    state.canUndo = false;
    
    Serial.printf("[SCORE] UNDO → Red: %d  Blue: %d\n", state.redScore, state.blueScore);
}

void score_reset(GameState &state) {
    score_init(state);
    Serial.println("[SCORE] GAME RESET");
}

bool score_checkWin(const GameState &state) {
    return state.gameOver;
}
