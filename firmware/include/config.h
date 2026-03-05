#ifndef CONFIG_H
#define CONFIG_H

// ============================================================
// LED Cornhole Scoreboard — Configuration
// ============================================================
// Change BOARD_ID and PEER_MAC for each board.
// Everything else should be identical on both boards.
// ============================================================

// --- Board Identity ---
// Set to BOARD_A or BOARD_B (the ONLY difference between boards)
#define BOARD_A  0
#define BOARD_B  1
#define BOARD_ID BOARD_A

// --- Peer MAC Address ---
// Set this to the OTHER board's WiFi MAC address
// Find it with: Serial.println(WiFi.macAddress());
#if BOARD_ID == BOARD_A
  #define BLE_DEVICE_NAME "Cornhole-A"
  static uint8_t PEER_MAC[] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF}; // Board B's MAC — update this!
#else
  #define BLE_DEVICE_NAME "Cornhole-B"
  static uint8_t PEER_MAC[] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF}; // Board A's MAC — update this!
#endif

// --- Game Settings ---
#define WINNING_SCORE    21
#define RESET_HOLD_MS    3000   // Hold reset button for 3 seconds to reset

// --- Pin Definitions (ESP32-S3) ---
#define PIN_LED_DATA     18     // WS2812B data line
#define PIN_BTN_RED      4      // Red team +1 button
#define PIN_BTN_BLUE     5      // Blue team +1 button
#define PIN_BTN_RESET    6      // Reset button (recessed)
#define PIN_BUZZER       7      // Passive buzzer

// --- LED Configuration ---
#define LEDS_PER_SEGMENT 4      // 4 LEDs per 7-segment segment (4" digits)
#define NUM_DIGITS       4      // 2 red digits + 2 blue digits
#define LEDS_PER_DIGIT   (7 * LEDS_PER_SEGMENT)  // 28 LEDs per digit
#define LEDS_SEPARATOR   4      // Colon dots between scores
#define LEDS_SCOREBOARD  (NUM_DIGITS * LEDS_PER_DIGIT + LEDS_SEPARATOR)  // 116

#define LEDS_HOLE_RING   24     // LEDs around the hole
#define LEDS_EDGE        0      // Edge/ambient LEDs (set to actual count when installed)

#define NUM_LEDS_TOTAL   (LEDS_SCOREBOARD + LEDS_HOLE_RING + LEDS_EDGE)

// --- LED Zones (starting index in the strip) ---
#define ZONE_SCOREBOARD_START  0
#define ZONE_HOLE_RING_START   LEDS_SCOREBOARD
#define ZONE_EDGE_START        (LEDS_SCOREBOARD + LEDS_HOLE_RING)

// --- Brightness ---
#define BRIGHTNESS_DEFAULT 150  // 0-255, ~60% (good balance of visibility and battery)
#define BRIGHTNESS_MIN     30
#define BRIGHTNESS_MAX     255

// --- Colors ---
#define COLOR_RED    CRGB(255, 0, 0)
#define COLOR_BLUE   CRGB(0, 80, 255)
#define COLOR_WHITE  CRGB(255, 255, 255)
#define COLOR_OFF    CRGB(0, 0, 0)

// --- Debounce ---
#define DEBOUNCE_MS  250  // Button debounce time in milliseconds

// --- ESP-NOW ---
#define ESPNOW_CHANNEL  0

#endif // CONFIG_H
