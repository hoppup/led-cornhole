#include "espnow_comm.h"
#include "../include/config.h"
#include <WiFi.h>
#include <esp_now.h>

// --- Receive buffer ---
static volatile bool _received = false;
static ScorePacket _rxPacket;

// --- Callbacks ---
static void onDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
    if (status == ESP_NOW_SEND_SUCCESS) {
        Serial.println("[ESP-NOW] Send: OK");
    } else {
        Serial.println("[ESP-NOW] Send: FAILED");
    }
}

static void onDataRecv(const esp_now_recv_info_t *info, const uint8_t *data, int len) {
    if (len == sizeof(ScorePacket)) {
        memcpy((void*)&_rxPacket, data, sizeof(ScorePacket));
        _received = true;
        Serial.printf("[ESP-NOW] Received: Red=%d Blue=%d Cmd=%d\n", 
                      _rxPacket.redScore, _rxPacket.blueScore, _rxPacket.command);
    } else {
        Serial.printf("[ESP-NOW] Received unexpected packet size: %d\n", len);
    }
}

void espnow_printMAC() {
    Serial.print("[ESP-NOW] This board's MAC: ");
    Serial.println(WiFi.macAddress());
}

void espnow_init() {
    // WiFi must be in STA mode for ESP-NOW
    WiFi.mode(WIFI_STA);
    WiFi.disconnect();
    
    espnow_printMAC();
    
    // Init ESP-NOW
    if (esp_now_init() != ESP_OK) {
        Serial.println("[ESP-NOW] ERROR: Init failed!");
        return;
    }
    
    // Register callbacks
    esp_now_register_send_cb(onDataSent);
    esp_now_register_recv_cb(onDataRecv);
    
    // Register peer (the other board)
    esp_now_peer_info_t peerInfo = {};
    memcpy(peerInfo.peer_addr, PEER_MAC, 6);
    peerInfo.channel = ESPNOW_CHANNEL;
    peerInfo.encrypt = ESPNOW_ENCRYPT;
    
    // Check if it's the default unset MAC (all 0xFF)
    bool macSet = false;
    for (int i = 0; i < 6; i++) {
        if (PEER_MAC[i] != 0xFF) { macSet = true; break; }
    }
    
    if (!macSet) {
        Serial.println("[ESP-NOW] WARNING: Peer MAC not set! Update PEER_MAC in config.h");
        Serial.println("[ESP-NOW] Running in standalone mode (no sync)");
    } else {
        if (esp_now_add_peer(&peerInfo) != ESP_OK) {
            Serial.println("[ESP-NOW] ERROR: Failed to add peer!");
        } else {
            Serial.printf("[ESP-NOW] Peer registered: %02X:%02X:%02X:%02X:%02X:%02X\n",
                         PEER_MAC[0], PEER_MAC[1], PEER_MAC[2],
                         PEER_MAC[3], PEER_MAC[4], PEER_MAC[5]);
        }
    }
    
    Serial.println("[ESP-NOW] Initialized");
}

void espnow_sendScore(const GameState &state) {
    ScorePacket pkt;
    pkt.redScore = state.redScore;
    pkt.blueScore = state.blueScore;
    pkt.round = state.round;
    pkt.command = CMD_SCORE_UPDATE;
    
    esp_now_send(PEER_MAC, (uint8_t*)&pkt, sizeof(pkt));
}

void espnow_sendReset() {
    ScorePacket pkt;
    pkt.redScore = 0;
    pkt.blueScore = 0;
    pkt.round = 0;
    pkt.command = CMD_RESET;
    
    esp_now_send(PEER_MAC, (uint8_t*)&pkt, sizeof(pkt));
}

bool espnow_hasReceived() {
    return _received;
}

ScorePacket espnow_getReceived() {
    _received = false;
    return _rxPacket;
}
