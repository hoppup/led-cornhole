#ifndef BLE_SERVICE_H
#define BLE_SERVICE_H

#include <Arduino.h>
#include "score.h"

// ============================================================
// BLE Service — STUB for future mobile app integration
// ============================================================
// This module will handle:
// - BLE GATT server (peripheral mode)
// - Score characteristic (read/write/notify)
// - Command characteristic (write)
// - Settings characteristic (read/write)
// - Auto-advertising as "Cornhole-A" or "Cornhole-B"
//
// For now, all functions are no-ops so the rest of the firmware
// compiles and runs without BLE. When ready, implement using
// the ESP32 BLE Arduino library.
// ============================================================

// Initialize BLE (currently a no-op stub)
void ble_init();

// Notify connected app of score change (currently a no-op stub)
void ble_notifyScore(const GameState &state);

// Check if BLE has received a command from the app (stub — always false)
bool ble_hasCommand();

// TODO: When implementing, add:
// - ble_update() for main loop
// - ble_getCommand() to retrieve app commands
// - ble_isConnected() to check app connection state

#endif
