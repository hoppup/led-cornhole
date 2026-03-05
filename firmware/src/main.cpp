/**
 * ============================================================
 * LED Cornhole Scoreboard — Main Firmware
 * ============================================================
 * 
 * ESP32-powered cornhole board with:
 * - WS2812B 7-segment LED scoreboard (4" digits, 4 LEDs/segment)
 * - ESP-NOW board-to-board wireless sync
 * - BLE mobile app connection (stubbed for future)
 * - Hole ring LED effects
 * - Buzzer sound effects
 * - Undo last score (hold both buttons)
 * 
 * Boards are peers — identical firmware, only BOARD_ID differs.
 * 
 * https://github.com/hoppup/led-cornhole
 * ============================================================
 */

#include <Arduino.h>
#include <FastLED.h>
#include "../include/config.h"
#include "score.h"
#include "display.h"
#include "buttons.h"
#include "espnow_comm.h"
#include "effects.h"
#include "buzzer.h"
#include "ble_service.h"

// --- Global State ---
CRGB leds[NUM_LEDS_TOTAL];
GameState game;

// Track if we just triggered a win (so we only celebrate once)
bool winCelebrated = false;

// ============================================================
// SETUP
// ============================================================
void setup() {
    Serial.begin(115200);
    delay(500);
    
    Serial.println();
    Serial.println("╔════════════════════════════════════╗");
    Serial.println("║   LED Cornhole Scoreboard v1.0     ║");
    Serial.printf( "║   Board: %-26s║\n", BLE_DEVICE_NAME);
    Serial.printf( "║   LEDs: %-4d total                 ║\n", NUM_LEDS_TOTAL);
    Serial.println("╚════════════════════════════════════╝");
    Serial.println();
    
    // --- Init LED strip ---
    FastLED.addLeds<WS2812B, PIN_LED_DATA, GRB>(leds, NUM_LEDS_TOTAL);
    FastLED.setBrightness(BRIGHTNESS_DEFAULT);
    display_init(leds);
    
    // --- Init subsystems ---
    buttons_init();
    buzzer_init();
    espnow_init();
    ble_init();         // Stub — ready for future app
    effects_init(leds);
    
    // --- Init game state ---
    score_init(game);
    
    // --- Startup sequence ---
    buzzer_startupSound();
    display_startupAnimation(leds);
    
    // Show initial score (0 : 0)
    display_updateScore(leds, game);
    
    Serial.println();
    Serial.println("[MAIN] Ready to play! Waiting for button presses...");
    Serial.println("[MAIN] Red button  → +1 Red");
    Serial.println("[MAIN] Blue button → +1 Blue");
    Serial.println("[MAIN] Both held   → Undo last");
    Serial.println("[MAIN] Reset held  → New game");
    Serial.println();
}

// ============================================================
// Handle a local score/command change
// (updates display, sends to peer, notifies BLE)
// ============================================================
void onScoreChanged(bool isRed) {
    // Visual feedback
    display_flashScore(leds, isRed);
    effects_scoreFlash(leds, isRed);
    
    // Sound
    buzzer_scoreBeep();
    
    // Update display
    display_updateScore(leds, game);
    
    // Sync to other board
    espnow_sendScore(game);
    
    // Notify app (stub for now)
    ble_notifyScore(game);
    
    // Check for win
    if (score_checkWin(game) && !winCelebrated) {
        winCelebrated = true;
        bool redWon = (game.redScore >= WINNING_SCORE);
        buzzer_winFanfare();
        effects_winCelebration(leds, redWon);
        // Restore score display after celebration
        display_updateScore(leds, game);
    }
}

void onUndo() {
    score_undo(game);
    winCelebrated = false;
    buzzer_undoBeep();
    display_updateScore(leds, game);
    espnow_sendScore(game);
    ble_notifyScore(game);
}

void onReset() {
    score_reset(game);
    winCelebrated = false;
    buzzer_resetSound();
    effects_resetAnimation(leds);
    display_updateScore(leds, game);
    espnow_sendReset();
    ble_notifyScore(game);
}

// ============================================================
// MAIN LOOP
// ============================================================
void loop() {
    // --- Poll buttons ---
    ButtonEvent btn = buttons_poll();
    
    switch (btn) {
        case BTN_RED_PRESS:
            score_addRed(game);
            onScoreChanged(true);
            break;
            
        case BTN_BLUE_PRESS:
            score_addBlue(game);
            onScoreChanged(false);
            break;
            
        case BTN_UNDO:
            onUndo();
            break;
            
        case BTN_RESET_HOLD:
            onReset();
            break;
            
        case BTN_NONE:
        default:
            break;
    }
    
    // --- Check for incoming ESP-NOW data from other board ---
    if (espnow_hasReceived()) {
        ScorePacket pkt = espnow_getReceived();
        
        switch (pkt.command) {
            case CMD_SCORE_UPDATE: {
                // Determine which team scored (compare to our current state)
                bool redChanged = (pkt.redScore != game.redScore);
                
                // Update our state to match
                game.redScore = pkt.redScore;
                game.blueScore = pkt.blueScore;
                game.round = pkt.round;
                game.gameOver = (game.redScore >= WINNING_SCORE || game.blueScore >= WINNING_SCORE);
                game.canUndo = false; // Can't undo remote changes
                
                Serial.printf("[MAIN] Remote score update: Red=%d Blue=%d\n", 
                             game.redScore, game.blueScore);
                
                // Visual + sound feedback
                buzzer_scoreBeep();
                display_updateScore(leds, game);
                
                // Check for win
                if (score_checkWin(game) && !winCelebrated) {
                    winCelebrated = true;
                    bool redWon = (game.redScore >= WINNING_SCORE);
                    buzzer_winFanfare();
                    effects_winCelebration(leds, redWon);
                    display_updateScore(leds, game);
                }
                break;
            }
                
            case CMD_RESET:
                Serial.println("[MAIN] Remote reset received");
                score_reset(game);
                winCelebrated = false;
                buzzer_resetSound();
                effects_resetAnimation(leds);
                display_updateScore(leds, game);
                break;
                
            default:
                Serial.printf("[MAIN] Unknown command: %d\n", pkt.command);
                break;
        }
    }
    
    // --- TODO: Check for BLE commands from app ---
    // if (ble_hasCommand()) { ... }
    
    // --- Update ambient effects (hole ring, edge LEDs) ---
    effects_update(leds, game);
    FastLED.show();
    
    // Small delay — prevents tight loop, saves power
    delay(10);
}
