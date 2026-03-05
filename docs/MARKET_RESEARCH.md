# LED Cornhole Scoreboard — Market & Prior Art Research

**Date:** 2026-03-05  
**Researched by:** Bob 🔨

---

## TL;DR

Yes, people have built this before — but **nobody has nailed the full package**. Most existing projects are missing key pieces (no app, no board-to-board sync, no ambient effects, ugly implementation). The closest commercial product (YoreBoard) sells for **$600-$1,000 per set**. There's a clear gap in the market for a well-executed, affordable version.

---

## 1. Commercial Products

### YoreBoard (yoreboard.com) — The Closest Competitor
- **What:** Complete cornhole boards with built-in Bluetooth LED scoreboard + app
- **Price:** $999 for finished boards, **$599 for DIY kit** (boards only, unfinished)
- **DIY Kit on Amazon:** ~$150-200 for electronics-only kit (1 board)
- **Features:**
  - Bluetooth connected
  - iOS + Android app (free)
  - LED scoring display
  - Ambiance mode lighting
  - Stainless steel LED buttons
  - 104 LEDs per kit
- **Weaknesses:**
  - Very expensive ($999 for a set of cornhole boards!)
  - DIY kit still costs $150-200 per board for just the electronics
  - No board-to-board wireless sync mentioned — seems app-dependent
  - Proprietary system, no customization
  - Limited LED effects

### ScoreMate by West Georgia Cornhole
- **What:** Cornhole scoring accessory (clips onto boards)
- **Features:** Scoring system, dual cup holders, LED spotlight, bottle opener
- **Not built-in** — it's an add-on attachment
- **No digital display** — just manual sliders with LED backlight

### SlickWoodys Bluetooth Speaker + Digital Scorekeeper
- **What:** Separate Bluetooth speaker with digital score display
- **Not integrated into the board** — standalone accessory
- **Includes speaker** (interesting feature idea)

### Generic LED Scoreboards (Amazon)
- Multiple brands selling standalone LED scoreboards ($30-60)
- Not cornhole-specific, not built into boards
- No wireless sync, no app
- Just basic up/down counters with buttons

### Etsy "Digital Scoreboard for Cornhole" by RT3Dimensions
- WiFi controlled, no app needed (uses captive portal)
- Standalone scoreboard, not built into boards
- LED digit display
- ~$80-100

---

## 2. DIY Projects (Closest to What We're Building)

### Instructables: "Cornhole LED Scoreboard Using an Arduino" (2021)
- **Link:** <https://www.instructables.com/Cornhole-LED-Scoreboard-Using-an-Arduino/>
- **YouTube:** <https://www.youtube.com/watch?v=Lv9JQUrU7GQ>
- **What:** WS2811 LEDs on cornhole board face, ESP8266 controller, 2 push buttons per board
- **Weaknesses:**
  - Uses ESP8266, not ESP32 (no BLE, no ESP-NOW)
  - No board-to-board communication — each board is independent
  - No mobile app
  - No ambient effects
  - Basic implementation, no 3D printed housings
- **What we can learn:** Their LED layout approach, mounting technique

### Reddit r/Cornhole: "Building a Light Up Cornhole Scoreboard" (2021)
- **Link:** <https://www.reddit.com/r/Cornhole/comments/ncd3ra/>
- A commenter built one **using ESP-NOW with ESP32** and 2 remotes
- Another commenter built one with **ESP-NOW syncing scores between both boards, 4 buttons on each board**
- **Parts listed:** WS2812B strips, Arduino Uno/ATmega128, voltage buck converter, 18650 batteries, push buttons
- **Weaknesses:**
  - No mobile app
  - No polished enclosure/housing
  - Never productized or shared code publicly

### Reddit r/esp32: "ESP32-Powered LED Matrix Scoreboard" (2024)
- **Link:** <https://www.reddit.com/r/esp32/comments/1b44cmd/>
- Uses 3 ESP32s + LED matrix display
- One ESP32 receives data, one per display
- Has Android app for control
- More complex (LED matrix instead of 7-segment)

### Reddit r/arduino: "Wireless Cornhole Scoring Board" (2014)
- **Link:** <https://www.reddit.com/r/arduino/comments/2py3v6/>
- One of the earliest — Arduino + LED matrix, wireless, battery powered
- Very basic, old hardware
- Shows this idea has been around 10+ years

### GitHub: zombodotcom/cornholeold
- **Link:** Referenced in Reddit comments
- ESP-NOW mesh version of cornhole scoreboard
- Creator mentioned wanting to start a company from it — never followed through
- Unfinished sketch, abandoned

### GitHub: scheerer/arduino-led-scoreboard
- **Link:** <https://github.com/scheerer/arduino-led-scoreboard>
- Arduino Mega based, uses custom PCB + Eagle schematics
- WS2812B LEDs, button input, font rendering
- **No wireless, no app** — single board only
- Good reference for 7-segment font mapping code

### GitHub: rtyle/artlight
- **Link:** <https://github.com/rtyle/artlight>
- ESP32-based "Art with Light" — supports cornhole + clock applications
- Uses SK9822 LEDs (similar to APA102, SPI-based)
- 1024 LEDs, TinyPICO board, light sensor for auto-brightness
- Very over-engineered for a scoreboard — more of an art project
- Custom PCBs, expensive BOM

### WLED Forum: "Building a Cornhole Score Board" (2023)
- **Link:** <https://wled.discourse.group/t/building-a-cornhole-score-board/8434>
- Someone trying to use WLED firmware for a cornhole scoreboard
- Wanted `88 : 88` display with 3 buttons
- Community response: WLED isn't ideal for this — better to write custom firmware
- **Confirms WLED is not the right approach** for our project

---

## 3. Mobile Apps (Software Only)

### Scoreholio
- **Link:** <https://scoreholio.com/>
- The dominant cornhole tournament management app
- Phone-only scoring — no hardware integration
- League management, brackets, stats
- No physical scoreboard connection

### The Cornhole Scorer (iOS)
- **Link:** App Store
- Simple phone-based scoring app
- No hardware integration
- Just a digital scorecard on your phone

---

## 4. Gap Analysis — What's Missing in the Market

| Feature | YoreBoard | DIY Projects | Our Build |
|---------|-----------|-------------|-----------|
| Built into board | ✅ | ✅ | ✅ |
| Board-to-board wireless sync | ❓ Unclear | Some (ESP-NOW) | ✅ ESP-NOW |
| Mobile app control | ✅ | ❌ Most don't | ✅ Flutter |
| LED ambient effects | ✅ Basic | ❌ | ✅ Advanced (hole ring, edge, animations) |
| 3D printed housings | ❌ | ❌ | ✅ |
| Sound effects | ❌ | ❌ | ✅ Buzzer |
| Auto-brightness | ❌ | 1 project (artlight) | ✅ Planned |
| Undo last score | ❌ | ❌ | ✅ |
| Open source | ❌ | Partial | ✅ |
| Win celebrations | ❌ | ❌ | ✅ |
| Cost per set | $600-1000 | $80-150 | **$130-230** |
| Battery powered | ✅ | Some | ✅ USB-C power bank |

---

## 5. What We Can Learn from Others

**Things that worked well:**
- ESP-NOW is proven for board-to-board sync (multiple people have used it successfully)
- WS2812B strips cut into segments is the standard approach
- 3-4 LEDs per segment is the sweet spot for visibility
- USB power banks or 18650 batteries are the go-to power solutions
- Physical buttons are essential (app-only control is frustrating during gameplay)

**Common mistakes to avoid:**
- Don't use WLED — it's not designed for scoreboard logic
- Don't use LED matrices (too many LEDs, too much power, overkill for a score display)
- Don't skip the 330Ω resistor on the data line (multiple projects report LED issues without it)
- Don't use ESP8266 — ESP32 is the right choice (ESP-NOW + BLE)
- Don't rely on WiFi — outdoor play often has no WiFi, ESP-NOW is the way

**Ideas to steal:**
- YoreBoard's "ambiance mode" — boards glow when not actively scoring (great for night parties)
- The 4-button-per-board concept (red+, red-, blue+, blue-) from a Reddit builder — though we're doing undo with button combo instead
- artlight's auto-brightness with a light sensor — definitely add this

---

## 6. Competitive Positioning (If We Sell)

**YoreBoard is the only real competitor** and they're at $600-1000. Our target:

| | YoreBoard | Our Kit (Electronics Only) | Our Complete Set |
|---|-----------|---------------------------|-----------------|
| **Price** | $150-200/board (DIY kit) | **$50-75/board** | **$150-200/set** |
| **Target** | Premium cornhole enthusiasts | DIY makers | Anyone who wants LED cornhole |
| **Differentiators** | Established brand | Open source, cheaper, more features | Better value, open source |

**Our advantages:**
1. **Price** — 50-70% cheaper than YoreBoard for similar features
2. **Open source** — community can contribute, customize, extend
3. **More effects** — hole ring, edge LEDs, win celebrations, sound
4. **ESP-NOW sync** — true peer-to-peer, doesn't depend on phone/app
5. **3D printable** — anyone with a printer can make the housings
6. **USB-C power** — universal, everyone has power banks

**Potential sell channels:**
- Etsy (DIY kits)
- Amazon (complete kits or finished boards)
- GitHub (open source builds community, drives kit sales)
- YouTube build videos (drive traffic to store)

---

*Last updated: 2026-03-05 by Bob 🔨*
