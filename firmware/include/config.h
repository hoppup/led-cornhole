#ifndef CONFIG_H
#define CONFIG_H

#include <FastLED.h>

// ============================================================
// LED Cornhole Scoreboard — Configuration
// ============================================================
// Change BOARD_ID and PEER_MAC for each board.
// Everything else should be identical on both boards.
// ============================================================

// --- Board Identity ---
#define BOARD_A  0
#define BOARD_B  1

// *** CHANGE THIS FOR EACH BOARD ***
#define BOARD_ID BOARD_A

// --- Peer MAC Address ---
// Find each board's MAC with: Serial.println(WiFi.macAddress());
// Then set the OTHER board's MAC here.
#if BOARD_ID == BOARD_A
  static const char* BLE_DEVICE_NAME = "Cornhole-A";
  static uint8_t PEER_MAC[] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF}; // ← Board B's MAC
#else
  static const char* BLE_DEVICE_NAME = "Cornhole-B";
  static uint8_t PEER_MAC[] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF}; // ← Board A's MAC
#endif

// --- Game Settings ---
#define WINNING_SCORE       21
#define UNDO_HOLD_MS        1000    // Hold both buttons 1s to undo last score
#define RESET_HOLD_MS       3000    // Hold both buttons 3s to reset game

// --- Pin Definitions ---
// Detect board type and assign pins accordingly
#ifdef CONFIG_IDF_TARGET_ESP32S3
  // ESP32-S3 DevKitC
  #define PIN_LED_DATA      18      // WS2812B data line (via 330Ω resistor)
  #define PIN_BTN_RED       4       // Red team +1 button (INPUT_PULLUP, active LOW)
  #define PIN_BTN_BLUE      5       // Blue team +1 button (INPUT_PULLUP, active LOW)
  #define PIN_BUZZER        7       // Passive buzzer
#else
  // Standard ESP32 (ESP32-D / DevKitC-32 / 38-pin)
  #define PIN_LED_DATA      18      // WS2812B data line (via 330Ω resistor)
  #define PIN_BTN_RED       4       // Red team +1 button (INPUT_PULLUP, active LOW)
  #define PIN_BTN_BLUE      5       // Blue team +1 button (INPUT_PULLUP, active LOW)
  #define PIN_BUZZER        15      // Passive buzzer
#endif
// No dedicated reset button — hold both red+blue for 3s to reset

// --- LED Layout ---
// Scoreboard: 4 digits × 7 segments × 4 LEDs/segment = 112, plus 4 separator dots = 116
#define LEDS_PER_SEGMENT    4
#define SEGMENTS_PER_DIGIT  7
#define LEDS_PER_DIGIT      (SEGMENTS_PER_DIGIT * LEDS_PER_SEGMENT)  // 28
#define NUM_DIGITS          4
#define LEDS_SEPARATOR      4       // 2 dots × 2 LEDs each
#define LEDS_SCOREBOARD     (NUM_DIGITS * LEDS_PER_DIGIT + LEDS_SEPARATOR)  // 116

#define LEDS_HOLE_RING      24
#define LEDS_EDGE           0       // Set when edge LEDs are installed

#define NUM_LEDS_TOTAL      (LEDS_SCOREBOARD + LEDS_HOLE_RING + LEDS_EDGE)

// --- LED Zone Start Indices ---
// Order on the strip: [Digit0][Digit1][Separator][Digit2][Digit3][HoleRing][Edge]
//   Digit 0 = Red tens, Digit 1 = Red ones, Digit 2 = Blue tens, Digit 3 = Blue ones
#define IDX_DIGIT_0         0
#define IDX_DIGIT_1         (IDX_DIGIT_0 + LEDS_PER_DIGIT)          // 28
#define IDX_SEPARATOR       (IDX_DIGIT_1 + LEDS_PER_DIGIT)          // 56
#define IDX_DIGIT_2         (IDX_SEPARATOR + LEDS_SEPARATOR)         // 60
#define IDX_DIGIT_3         (IDX_DIGIT_2 + LEDS_PER_DIGIT)          // 88
#define IDX_HOLE_RING       LEDS_SCOREBOARD                          // 116
#define IDX_EDGE            (IDX_HOLE_RING + LEDS_HOLE_RING)        // 140

// --- Brightness ---
#define BRIGHTNESS_DEFAULT  150     // ~60% — good for battery life + visibility
#define BRIGHTNESS_MIN      30
#define BRIGHTNESS_MAX      255

// --- Colors ---
#define COLOR_RED           CRGB(255, 0, 0)
#define COLOR_BLUE          CRGB(0, 80, 255)
#define COLOR_WHITE         CRGB(255, 255, 255)
#define COLOR_GREEN         CRGB(0, 255, 0)
#define COLOR_PURPLE        CRGB(128, 0, 255)
#define COLOR_OFF           CRGB(0, 0, 0)
#define COLOR_DIM_RED       CRGB(40, 0, 0)
#define COLOR_DIM_BLUE      CRGB(0, 10, 40)

// --- Timing ---
#define DEBOUNCE_MS         250
#define SCORE_FLASH_MS      300     // Flash duration on score
#define WIN_ANIM_DURATION   5000    // Win celebration length

// --- ESP-NOW ---
#define ESPNOW_CHANNEL      0
#define ESPNOW_ENCRYPT      false

// --- Packet Commands ---
#define CMD_SCORE_UPDATE    0
#define CMD_RESET           1
#define CMD_EFFECT          2

#endif // CONFIG_H
