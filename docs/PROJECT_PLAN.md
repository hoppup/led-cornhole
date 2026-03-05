# LED Cornhole Scoreboard — Project Plan

**Project:** ESP32-powered cornhole boards with built-in LED scoreboards  
**Started:** 2026-03-05  
**Status:** Planning  
**Digit Size:** 4" tall (4 LEDs per segment) — chosen for distance readability

---

## 1. Overview

Build a set of two cornhole boards, each with:
- A WS2812B LED strip 7-segment scoreboard on the face (**4" tall digits, 4 LEDs/segment**)
- Red and blue push buttons for scoring
- ESP-NOW wireless sync between boards (no WiFi network needed)
- A mobile app for remote score control
- Decorative LED effects (hole ring, ambient lighting)

Both boards display the same score at all times. Either board's buttons (or the app) can update the score.

### Board Naming Convention
The two boards are **peers** — identical firmware, identical capabilities.

| Board | BLE Name | Role |
|-------|----------|------|
| **Board A** | `Cornhole-A` | Peer — identical to Board B |
| **Board B** | `Cornhole-B` | Peer — identical to Board A |

The only firmware difference is a single config value (`BOARD_ID`) that determines:
- Which MAC address to pair with (the *other* board)
- BLE advertising name (`Cornhole-A` or `Cornhole-B`)
- Optional: small status LED color to visually identify which is which

```cpp
// config.h — the ONLY difference between Board A and Board B firmware
#define BOARD_ID        BOARD_A        // Change to BOARD_B for the other board
#define PEER_MAC        {0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF}  // Other board's MAC
```

---

## 2. Board-to-Board Communication: ESP-NOW

**ESP-NOW** is the right protocol for this. Here's why:

- **Built into every ESP32** — no extra hardware needed
- **Peer-to-peer** — no WiFi router or internet required (works outdoors, at tailgates, anywhere)
- **Fast** — sub-10ms latency, scores update instantly on both boards
- **Low power** — important for battery operation
- **Simple** — just pair the two boards by MAC address, send/receive structs
- **Range** — ~200m line of sight (more than enough for cornhole distance of 27 feet)
- **Reliable** — includes delivery callbacks so you know the other board received the update

**How it works:**
1. Each ESP32 registers the other as a peer (by MAC address)
2. When a button is pressed, it sends a score update packet via ESP-NOW
3. The receiving board updates its display
4. Both boards maintain the same score state

**Data packet (simple struct):**
```c
typedef struct {
    uint8_t redScore;
    uint8_t blueScore;
    uint8_t round;       // optional: track rounds
    uint8_t command;     // 0=score_update, 1=reset, 2=effect_trigger
} ScorePacket;
```

---

## 3. Hardware — Per Board

### Microcontroller
| Component | Recommendation | Notes |
|-----------|---------------|-------|
| **ESP32 board** | ESP32-S3 DevKitC or ESP32-WROOM-32 | S3 has more GPIO and better BLE 5.0 for the app |

**Recommendation: Go with ESP32-S3** — better BLE support for the mobile app + more pins for all the LED zones.

### LED Strips (WS2812B, 5V, 60 LEDs/m, black PCB, IP30)
| Zone | LEDs Needed | Strip Length | Purpose |
|------|-------------|-------------|---------|
| **Scoreboard digits** | 112 (4 digits × 7 seg × 4 LEDs) | ~1.9m | Two 2-digit 7-segment displays (red + blue), **4 LEDs per segment** |
| **Separator dots** | 4 (2 dots × 2 LEDs) | minimal | Colon between red and blue scores |
| **Hole ring** | 24 | ~0.4m | Ring of LEDs around the cornhole hole |
| **Ambient/edge** | 30-60 (optional) | ~0.5-1.0m | Decorative strips along board edges or underglow |
| **Total per board** | **~170-200** | **~2.8-3.3m** | All on one data line (daisy-chained) |
| **Total for 2 boards** | **~340-400** | **~5.6-6.6m** | **2× 5m rolls recommended** (plenty of spares) |

### Buttons & Controls
| Component | Qty | Notes |
|-----------|-----|-------|
| **Red arcade button** | 1 | Score +1 for red team |
| **Blue arcade button** | 1 | Score +1 for blue team |
| **Reset button** | 1 | Small tactile button (recessed so it's not hit accidentally) |
| **10K pull-down resistors** | 3 | One per button |

### Power
| Component | Notes |
|-----------|-------|
| **5V USB-C power bank** | 10,000–20,000 mAh — runs a full day of games easily |
| **5V buck converter** | If using a larger battery pack (e.g., 12V for brighter LEDs) |
| **Capacitor (1000µF)** | Across LED strip power to prevent inrush damage |
| **Resistor (330Ω)** | On the data line to first LED |

### Wiring Summary
```
USB-C Power Bank (5V)
  ├── ESP32 (via USB or VIN)
  ├── WS2812B Strip (5V + GND)
  │     └── Data pin → ESP32 GPIO (via 330Ω resistor)
  ├── Red Button → GPIO + 10K pull-down
  ├── Blue Button → GPIO + 10K pull-down
  └── Reset Button → GPIO + 10K pull-down
```

---

## 4. Scoreboard Display Design

### 7-Segment Layout
Each board displays **4 digits**: `RR : BB` (red score, blue score)

```
 ___     ___         ___     ___
|   |   |   |   •   |   |   |   |
|___|   |___|   •   |___|   |___|
|   |   |   |       |   |   |   |
|___|   |___|       |___|   |___|
 RED     RED        BLUE    BLUE
```

**Chosen size: 4" tall digits**
- **4 LEDs per segment** × 7 segments = **28 LEDs per digit**
- 4 digits × 28 = **112 LEDs** for the score display
- Separator dots: **4 LEDs** (2 dots × 2 LEDs each)
- **Total per board: 116 LEDs (~1.94m of strip)**
- Red team digits display in RED, blue team digits display in BLUE
- Easily readable from 27+ feet (regulation cornhole distance)

### 3D Printed Segment Housings
Since you have a 3D printer, this is the move:
- Print individual segment housings (diffused covers for clean look)
- **Thingiverse has ready-made designs** — search "WS2812B 7 segment"
- Mount segments on the board face with screws or adhesive
- White/frosted PETG or PLA for the diffuser covers (spreads light evenly)

### Existing Library
**PA0DEV/WS2812b-7-Segment-Display** on GitHub — MicroPython library that handles:
- Configurable LEDs per segment
- Multiple digit instances on one data line
- Individual digit color control (perfect for red/blue team colors)

We can port this to Arduino/C++ or write our own (it's straightforward mapping).

---

## 5. Software Architecture (ESP32 Firmware)

### Framework
**Arduino IDE with PlatformIO** (or just Arduino IDE for simplicity)

### Key Libraries
- **FastLED** — WS2812B control (better than Adafruit NeoPixel for effects)
- **ESP-NOW** — built into ESP32 Arduino core
- **BLE** — built into ESP32 Arduino core (for mobile app)
- **Preferences** — built-in NVS storage (save scores on power loss)

### Firmware Flow
```
BOOT
  ├── Init FastLED (all LED zones)
  ├── Init ESP-NOW (register peer board)
  ├── Init BLE (advertise for mobile app)
  ├── Load last scores from NVS (optional)
  └── Show startup animation
  
MAIN LOOP
  ├── Check buttons (debounced)
  │     ├── Red pressed → redScore++ → update display → send ESP-NOW → notify BLE
  │     ├── Blue pressed → blueScore++ → update display → send ESP-NOW → notify BLE
  │     └── Reset held 3s → reset scores → update display → send ESP-NOW → notify BLE
  ├── Check ESP-NOW incoming
  │     └── Score update received → update display → notify BLE
  ├── Check BLE incoming
  │     └── Score update from app → update display → send ESP-NOW
  └── Run LED animations (hole ring effects, ambient)
```

### Score Logic
- Standard cornhole: first to **21** wins
- Scores can go up only (no negative)
- Optional: **cancellation scoring** (only the difference scores each round) — can be a mode toggle
- Reset requires **3-second hold** on reset button (prevents accidental reset)
- Win detection: flash celebration animation when a team hits 21

---

## 6. Mobile App

### Recommended Approach: Flutter + BLE

**Why Flutter:**
- Single codebase → iOS + Android
- Great BLE libraries (`flutter_blue_plus`)
- You can build it yourself or I can help scaffold it
- Clean Material Design UI out of the box

**Alternative:** React Native with `react-native-ble-plx` (also good, but Flutter's BLE support is more mature)

### BLE Architecture — Single Connection with Failover

The app connects to **one board at a time** (not both). That board relays all updates to the other board via ESP-NOW.

```
Phone App ──BLE──► Board A ──ESP-NOW──► Board B
                      │                     │
                   Display              Display
                   updated              updated
```

**Why single connection:**
- Simpler app code — one BLE connection to manage
- No sync conflicts — single source of truth
- Less phone battery drain
- If the connected board goes down, the app auto-scans and **falls back to the other board**

**Connection flow:**
1. App scans for BLE devices named `Cornhole-A` or `Cornhole-B`
2. Connects to whichever it finds first (or user picks)
3. All score updates go through that one BLE link
4. Connected board relays to the other via ESP-NOW
5. If BLE disconnects, app auto-reconnects or falls back to the other board

### BLE GATT Service
- ESP32 acts as a **BLE peripheral** (GATT server)
- App connects as the **central** device
- Simple characteristics:
  - **Score characteristic** (read/write/notify): `{red: 12, blue: 9, round: 4}`
  - **Command characteristic** (write): reset, change mode, trigger effects
  - **Settings characteristic** (read/write): brightness, animation mode, game-to score

### App Features
**Core:**
- Auto-scan and connect to nearest board (`Cornhole-A` or `Cornhole-B`)
- Display current scores (big, clear numbers)
- +1 buttons for red and blue teams
- Reset game button (with confirmation)
- Score history for the session
- Auto-failover to other board if connection drops

**Nice to Have:**
- Player name entry
- Game timer
- Win/loss tracking across multiple games
- Choose LED effects/colors from the app
- Brightness control
- "Tournament mode" with bracket tracking
- Share scores / take photo with score overlay

### BLE Range Note
BLE range is ~30-50 feet reliably, which covers both boards since they're only 27 feet apart. Since the app only connects to one board, you just need to be within range of either one. The ESP-NOW link handles board-to-board sync independently.

---

## 7. LED Effects & Extras

### Hole Ring LEDs
- **Ring of 20-30 LEDs** around the cornhole hole
- Effects:
  - **Steady glow** — team colors split (half red, half blue)
  - **Pulse on score** — flash the scoring team's color
  - **Cornhole celebration** — rainbow chase when a bag goes through
  - **Idle rainbow** — slow rainbow cycle when game isn't active

### Ambient/Edge LEDs
- Strips along the edges or underside of the board
- **Underglow** — makes the boards look incredible at night
- Can match team colors or run independent animations
- Brightness adjustable via app

### Score Animations
- **Score increment** — quick flash/pulse when score changes
- **Win celebration** — fireworks/rainbow chase when a team hits 21
- **Game start** — countdown animation
- **Overtime alert** — pulsing when both teams are close to 21

### Bag Detection (Future Enhancement)
- **IR break-beam sensor** inside the hole to auto-detect cornholes (+3 points)
- Could also add **pressure sensors** on the board surface for bag-on-board detection (+1 point)
- This would make scoring fully automatic — no buttons needed

---

## 8. Physical Build

### Board Construction
- Standard cornhole board dimensions: **2ft × 4ft**, hole is **6 inches** diameter, **9 inches** from top
- LED scoreboard area: top section of the board face (above the hole)
- Buttons: mounted on the side rail of the board (easy to reach, won't get hit by bags)
- ESP32 + wiring: enclosed box on the back or inside the board frame
- Power bank: velcro-mounted on back for easy removal/charging

### 3D Printed Parts
- 7-segment digit housings with diffuser
- Button mounts / bezels
- ESP32 enclosure
- Hole ring LED channel/diffuser
- Wire management clips

### Weather Considerations
- Cornhole boards are often used outdoors
- Conformal coat or silicone on ESP32 and connections
- LED strips in channels with clear covers
- Buttons rated for outdoor use (IP65 arcade buttons exist)
- Store power bank inside board frame (protected)

---

## 9. Bill of Materials (Estimated per Set of 2 Boards)

| Item | Qty | Est. Cost | Notes |
|------|-----|-----------|-------|
| ESP32-S3 DevKitC | 2 | $16 | ~$8 each |
| WS2812B LED strip (60/m, 5m) | 2 | $24 | ~$12 each, one strip per board |
| 30mm Arcade buttons (red) | 2 | $6 | One per board |
| 30mm Arcade buttons (blue) | 2 | $6 | One per board |
| Tactile reset buttons | 2 | $2 | |
| 10K resistors | 6 | $1 | |
| 330Ω resistors | 2 | $1 | Data line protection |
| 1000µF capacitors | 2 | $2 | |
| USB-C power banks (10000mAh) | 2 | $30 | ~$15 each |
| Wire (22AWG silicone) | 1 roll | $8 | |
| Connectors (JST, barrel) | misc | $5 | |
| 3D printing filament (PETG) | ~200g | $5 | For segments + housings |
| Cornhole boards | 1 set | $0-80 | Build or buy unfinished |
| **Total** | | **~$106-186** | |

---

## 10. Development Phases

### Phase 1: Prototype (Week 1-2)
- [ ] Order ESP32-S3 boards + LED strips + buttons
- [ ] 3D print one set of 7-segment housings
- [ ] Wire up ONE board on a breadboard
- [ ] Write basic firmware: buttons → score → LED display
- [ ] Test 7-segment display logic

### Phase 2: Board-to-Board Sync (Week 2-3)
- [ ] Get ESP-NOW working between two ESP32s
- [ ] Send/receive score packets
- [ ] Build second board prototype
- [ ] Test bidirectional sync (both boards update from either side)

### Phase 3: Mobile App (Week 3-5)
- [ ] Flutter app scaffold with BLE
- [ ] Connect to ESP32, read/write scores
- [ ] Score display UI
- [ ] Score control buttons
- [ ] Test app ↔ ESP32 ↔ ESP-NOW ↔ second board (full chain)

### Phase 4: Effects & Polish (Week 5-6)
- [ ] Hole ring LED animations
- [ ] Ambient/edge lighting
- [ ] Win celebration animations
- [ ] Brightness controls
- [ ] Power optimization

### Phase 5: Physical Build (Week 6-8)
- [ ] Design final 3D printed parts
- [ ] Build/finish cornhole boards
- [ ] Mount all electronics
- [ ] Wire management
- [ ] Weatherproofing
- [ ] Final testing

---

## 11. Bob's Suggestions

### 🔥 Must-Do
1. **Use ESP32-S3** over the regular ESP32 — better BLE 5.0 for the app, more GPIOs, and USB-C native
2. **Power from USB-C power banks** — everyone has them, easy to charge, no custom battery wiring
3. **3D print the segment housings** — you already have the printer, and it'll look 10x better than bare strips
4. **Add a startup animation** — when boards power on, run a quick LED test/chase. Sets the vibe.

### 💡 Nice Additions
5. **Sound effects** — small buzzer or speaker for score beeps, win fanfare (cheap and adds a lot)
6. **Score-by-round tracking** — store each round's throws so you can review the game from the app
7. **"Last bag" undo button** — hold blue+red together for 1 sec to undo the last score change (mis-presses happen)
8. **Auto-brightness** — add a photoresistor so the LEDs dim in low light and brighten in sunlight
9. **Team name display** — if you use enough LEDs, scroll team names across the display between games
10. **NFC tap to connect** — ESP32-S3 supports NFC; tap your phone to instantly connect the app (no manual BLE pairing)

### 🎯 Future Upgrades
11. **Bag detection sensors** — IR break-beam in the hole + weight sensors on the board = fully automatic scoring
12. **WiFi mode** — at home, boards connect to WiFi and post scores to a web dashboard / TV display
13. **Tournament bracket** — app manages multiple teams, tracks wins, generates brackets
14. **LED board skins** — different color themes (patriotic, team colors, holiday themes) selectable from app
15. **Sell kits** — once you nail the design, this is absolutely a sellable product. ESP32 + 3D printed parts + LED strips as a kit for people who already have cornhole boards.

---

## 12. Reference Projects & Resources

- **Hackster.io ESP32 7-Segment Scoreboard** — very similar project with 3D printed segments and buttons
  - <https://www.hackster.io/jlorentz/esp32-7-segment-scoreboard-33534f>
- **WS2812b 7-Segment Display Library** (MicroPython) — digit mapping logic
  - <https://github.com/PA0DEV/WS2812b-7-Segment-Display>
- **ESP-NOW Guide** — Random Nerd Tutorials (best ESP32 tutorial site)
  - <https://randomnerdtutorials.com/esp-now-esp32-arduino-ide/>
- **3D Printable 7-Segment Display** — Thingiverse
  - <https://www.thingiverse.com/thing:3095715>
- **FastLED Library** — LED control
  - <https://github.com/FastLED/FastLED>
- **Flutter BLE Plus** — mobile app BLE library
  - <https://pub.dev/packages/flutter_blue_plus>

---

## File Structure (When We Start Coding)
```
led-cornhole/
├── PROJECT_PLAN.md          ← this file
├── firmware/
│   ├── platformio.ini
│   └── src/
│       ├── main.cpp         ← entry point
│       ├── display.h/cpp    ← 7-segment LED control
│       ├── espnow.h/cpp     ← board-to-board comms
│       ├── ble.h/cpp        ← mobile app comms
│       ├── buttons.h/cpp    ← button input handling
│       ├── effects.h/cpp    ← LED animations
│       └── config.h         ← pin definitions, BOARD_ID (Home/Away), peer MAC
├── app/
│   └── cornhole_app/        ← Flutter project
├── 3d-models/
│   ├── segment_housing.stl
│   ├── button_mount.stl
│   ├── esp32_enclosure.stl
│   └── hole_ring_channel.stl
└── docs/
    ├── wiring_diagram.md
    └── photos/
```

---

*Last updated: 2026-03-05 by Bob 🔨*
