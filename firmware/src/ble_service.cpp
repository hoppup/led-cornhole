#include "ble_service.h"
#include "../include/config.h"

// ============================================================
// BLE Service — STUB (no-op implementations)
// Replace these with real BLE GATT server code when ready.
// ============================================================

void ble_init() {
    Serial.println("[BLE] Stub initialized (mobile app support not yet implemented)");
    Serial.printf("[BLE] Will advertise as: %s\n", BLE_DEVICE_NAME);
    // TODO: Initialize BLE with:
    // - BLEDevice::init(BLE_DEVICE_NAME)
    // - Create GATT service with score/command/settings characteristics
    // - Start advertising
}

void ble_notifyScore(const GameState &state) {
    // TODO: Notify connected BLE client of score update
    // For now, just log that we would have sent it
    (void)state; // Suppress unused warning
}

bool ble_hasCommand() {
    // TODO: Check if BLE client sent a command
    return false;
}
