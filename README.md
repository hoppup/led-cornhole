# 🌽 LED Cornhole Scoreboard

ESP32-powered cornhole boards with built-in LED scoreboards, wireless board-to-board sync, and a mobile app.

![Status](https://img.shields.io/badge/status-planning-yellow)
![License](https://img.shields.io/badge/license-MIT-blue)

## Features

- **Built-in 7-segment LED scoreboard** on each board face (4" digits, WS2812B strips embedded in epoxy)
- **ESP-NOW wireless sync** — both boards always show the same score, no WiFi needed
- **Bluetooth mobile app** (Flutter) — control scores from your phone
- **LED effects** — hole ring glow, edge ambient lighting, win celebrations
- **Battery powered** — USB-C power bank, plays all day
- **Premium build** — 3/4" Baltic Birch, poplar frame, regulation ACL dimensions

## How It Works

```
Phone App ──BLE──► Board A ──ESP-NOW──► Board B
                      │                     │
                   Display              Display
                   Buttons              Buttons
```

- Either board's buttons update the score on both boards
- App connects to one board via BLE, which relays to the other via ESP-NOW
- If connection drops, app auto-fails over to the other board

## Hardware

| Component | Per Board |
|-----------|-----------|
| ESP32-S3 DevKitC | 1 |
| WS2812B LED strip (5V, 60 LED/m) | ~3m |
| 30mm LED arcade buttons (red + blue) | 2 |
| Tactile reset button | 1 |
| USB-C power bank (10,000mAh) | 1 |
| Passive buzzer | 1 |

**Scoreboard:** 4 digits × 7 segments × 4 LEDs = 112 LEDs + 4 separator dot LEDs = **116 LEDs per board**

## Project Structure

```
led-cornhole/
├── README.md
├── firmware/           ← ESP32 Arduino/PlatformIO code
│   ├── src/
│   │   ├── main.cpp
│   │   ├── display.h/cpp
│   │   ├── espnow_comm.h/cpp
│   │   ├── ble_service.h/cpp
│   │   ├── buttons.h/cpp
│   │   ├── effects.h/cpp
│   │   └── score.h/cpp
│   ├── include/
│   │   └── config.h
│   └── platformio.ini
├── app/                ← Flutter mobile app
│   └── cornhole_app/
├── 3d-models/          ← Printable parts (STL)
├── hardware/           ← Wiring diagrams, schematics
└── docs/               ← Build guides, reference
    ├── PROJECT_PLAN.md
    ├── PARTS_LIST.md
    ├── BOARD_BUILD_PLAN.md
    └── MARKET_RESEARCH.md
```

## Docs

- [Project Plan](docs/PROJECT_PLAN.md) — Full technical plan
- [Parts List & Cost Sheet](docs/PARTS_LIST.md) — BOM with Amazon/AliExpress pricing
- [Board Build Plan](docs/BOARD_BUILD_PLAN.md) — Premium cornhole board construction guide
- [Market Research](docs/MARKET_RESEARCH.md) — Competitive analysis & prior art

## Development Phases

1. **Prototype** — Single board on breadboard, buttons → score → LED display
2. **Board-to-Board Sync** — ESP-NOW between two ESP32s
3. **Mobile App** — Flutter + BLE score control
4. **Effects & Polish** — Animations, sound, brightness
5. **Physical Build** — Premium boards with embedded LEDs

## Cost

| Build Type | Est. Cost (2 boards) |
|-----------|---------------------|
| Electronics (Amazon) | ~$175 |
| Premium board build | ~$235 |
| **Total** | **~$410** |
| Comparable product (YoreBoard) | $600–1,000 |

## License

MIT — build your own, sell your own, have fun.

---

*Built by Justin Hopp & Bob 🔨*
