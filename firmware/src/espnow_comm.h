#ifndef ESPNOW_COMM_H
#define ESPNOW_COMM_H

#include <Arduino.h>
#include "score.h"

// Packet sent between boards
struct ScorePacket {
    uint8_t redScore;
    uint8_t blueScore;
    uint8_t round;
    uint8_t command;    // CMD_SCORE_UPDATE, CMD_RESET, CMD_EFFECT
};

// Initialize ESP-NOW and register peer
void espnow_init();

// Send current score to the other board
void espnow_sendScore(const GameState &state);

// Send a reset command to the other board
void espnow_sendReset();

// Check if a new packet has been received (call from loop)
bool espnow_hasReceived();

// Get the last received packet (clears the received flag)
ScorePacket espnow_getReceived();

// Print this board's MAC address (for setup)
void espnow_printMAC();

#endif
