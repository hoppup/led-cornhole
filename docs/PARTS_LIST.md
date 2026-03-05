# LED Cornhole Scoreboard — Parts List & Cost Sheet

**Last Updated:** 2026-03-05  
**Pricing:** Amazon (fast) vs AliExpress (bulk/resale)  
**Quantities:** For **1 set of 2 boards** (personal build)

---

## Quick Summary

| Source | Prototype Cost | Notes |
|--------|---------------|-------|
| **Amazon (fast ship)** | **~$135–165** | Get parts in 2-5 days |
| **AliExpress (budget)** | **~$70–95** | 2-4 week shipping |
| **Resale build (AliExpress bulk)** | **~$55–75** | Buying 10+ sets of components |

> **Note:** Justin already has ESP32s on hand for dev/testing. Final production boards (ESP32-S3) are in the list for the finished build.

---

## ELECTRONICS — Per Set (2 Boards)

### Microcontrollers

| Part | Spec | Qty | Amazon | AliExpress | Links/Notes |
|------|------|-----|--------|------------|-------------|
| ESP32-S3 DevKitC-1 | N16R8 (16MB flash, 8MB PSRAM) | 2 | ~$10 ea / **$20** | ~$5 ea / **$10** | 2-pack on Amazon ~$18. Go N16R8 for BLE + OTA headroom |
| ESP32 (dev/testing) | Any WROOM-32 you have | 2 | **$0** (on hand) | — | Use these for prototyping, swap to S3 for final |

### LED Strips

| Part | Spec | Qty | Amazon | AliExpress | Links/Notes |
|------|------|-----|--------|------------|-------------|
| WS2812B LED Strip | 5V, 60 LED/m, IP30, black PCB | 2× 5m rolls | ~$14 ea / **$28** | ~$6-8 ea / **$12-16** | BTF-LIGHTING or Aclorol on Amazon. IP30 (non-waterproof) is fine — we'll protect with 3D printed housings |
| WS2812B LED Strip (alt: 1m cuts) | 5V, 60 LED/m, IP30 | 4× 1m | ~$5 ea / **$20** | ~$3 ea / **$12** | If you want shorter strips to reduce waste. Each board needs ~2-3m total |

**LED count breakdown per board:**
- Scoreboard digits: 4 digits × 7 segments × 3-4 LEDs = **84-112 LEDs** (~1.5-2m)
- Separator dots: **4 LEDs**
- Hole ring: **24 LEDs** (~0.4m)
- Edge/ambient: **30-60 LEDs** (~0.5-1m, optional)
- **Total per board: ~142-200 LEDs (~2.5-3.5m)**
- **Total for 2 boards: ~284-400 LEDs (~5-7m)**

**Recommendation:** Get 2× 5m rolls. Gives you 600 LEDs / 10m total — enough for both boards with plenty of spares for testing and mistakes.

### Buttons

| Part | Spec | Qty | Amazon | AliExpress | Links/Notes |
|------|------|-----|--------|------------|-------------|
| 30mm LED Arcade Button (RED) | 5V illuminated, built-in switch | 2 | ~$3 ea / **$6** | ~$1 ea / **$2** | EG STARTS brand on Amazon. 5V LED matches ESP32 power |
| 30mm LED Arcade Button (BLUE) | 5V illuminated, built-in switch | 2 | ~$3 ea / **$6** | ~$1 ea / **$2** | Same brand, blue |
| Tactile Push Button (Reset) | 6mm, momentary | 2 | **$1** (pack) | **$0.50** | Recessed mount — prevents accidental resets |
| **Button option B:** 5-pack mixed | 30mm, 5 colors, 5V LED | 1 pack | **$8** | **$4** | EG STARTS 5-pack — you get red, blue + spares in other colors |

### Passive Components

| Part | Spec | Qty | Amazon | AliExpress | Links/Notes |
|------|------|-----|--------|------------|-------------|
| 330Ω Resistors | 1/4W, through-hole | 4+ | **$1** (pack of 100) | **$0.50** | Data line protection for LED strips. One per strip segment |
| 10KΩ Resistors | 1/4W, through-hole | 6+ | **$1** (pack of 100) | **$0.50** | Pull-down for buttons (if not using internal pull-ups) |
| 1000µF Electrolytic Capacitor | 6.3V or higher | 2 | **$2** (pack of 10) | **$1** | Across LED strip power input — prevents inrush spikes |
| Resistor/Cap Assortment Kit | Mixed values | 1 | **$10-13** | **$5** | Worth getting if you don't have a stash. Covers this + future projects |

### Connectors & Wiring

| Part | Spec | Qty | Amazon | AliExpress | Links/Notes |
|------|------|-----|--------|------------|-------------|
| JST SM 3-Pin Connectors | 22AWG, male+female pairs | 1 pack (20 pairs) | **$8** | **$3** | BTF-LIGHTING pack. Makes LED strips modular/disconnectable |
| 22AWG Silicone Wire | 6-color kit, 26ft each | 1 kit | **$12** | **$5** | TUOFENG or BOJACK brand. Flexible, heat-resistant, great for tight spaces |
| Dupont Jumper Wires | M-M, M-F, F-F assorted | 1 kit | **$7** | **$2** | For prototyping/breadboard phase |
| Heat Shrink Tubing | Assorted sizes | 1 kit | **$7** | **$3** | For clean solder joints |
| PCB Screw Terminal Blocks | 2-pin and 3-pin, 5.08mm | 10 | **$5** | **$2** | For connecting power + buttons to a perfboard |

### Power

| Part | Spec | Qty | Amazon | AliExpress | Links/Notes |
|------|------|-----|--------|------------|-------------|
| USB-C Power Bank | 10,000mAh, 5V/2A+ output | 2 | ~$13 ea / **$26** | ~$8 ea / **$16** | Anker PowerCore or similar. Must have steady 5V/2A output |
| USB-C to bare wire cable | 5V power tap | 2 | **$6** | **$2** | Cut and solder to power LED strips directly. Or use USB breakout boards |
| USB-C Breakout Board | 5V output, screw terminals | 2 | ~$4 ea / **$8** | ~$1.50 ea / **$3** | Cleaner than cutting cables. Search "USB-C PD trigger 5V" |

**Power math:**
- ~200 LEDs × 60mA max (full white) = 12A max (won't happen in practice)
- Typical use (colored scores, not full white): ~2-3A per board
- 10,000mAh ÷ 2.5A = ~4 hours continuous, plenty for a game session
- Add brightness limiting in firmware (50-70% max) to extend battery life

### Sound (Optional but Recommended)

| Part | Spec | Qty | Amazon | AliExpress | Links/Notes |
|------|------|-----|--------|------------|-------------|
| Passive Buzzer Module | 5V, 3-pin | 2 | ~$3 ea / **$6** | **$1.50** | Score beeps, win fanfare. Passive > active (can play tones/melodies) |
| **OR** Small Speaker + Amp | MAX98357A I2S amp + 3W speaker | 2 sets | ~$8 ea / **$16** | ~$3 ea / **$6** | Better sound quality if you want real audio effects. Works with ESP32 I2S |

---

## 3D PRINTING

| Part | Material | Qty | Cost | Notes |
|------|----------|-----|------|-------|
| PETG Filament (translucent white) | 1.75mm, 1kg spool | 1 | **$20** (Amazon) / **$12** (AliExpress) | For LED diffuser covers on 7-segment digits. Translucent PETG diffuses light beautifully |
| PLA/PETG Filament (black) | 1.75mm, 1kg spool | 1 | **$18** (Amazon) / **$10** (AliExpress) | For housings, button mounts, ESP32 enclosure |
| **Filament estimate** | | | | You'll use maybe 200-400g total. Likely have some on hand already |

**3D Printed Parts List:**
- 8× 7-segment digit housings (with diffuser slots)
- 4× Separator dot housings
- 4× Button mount bezels (arcade button cutouts)
- 2× ESP32 enclosures (with ventilation)
- 2× Hole ring LED channels (circular diffuser)
- 2× Wire routing clips/mounts
- 2× Battery bank mounting brackets

---

## BOARD CONSTRUCTION (Premium Build)

| Part | Spec | Qty | Est. Cost | Notes |
|------|------|-----|-----------|-------|
| **3/4" Baltic Birch Plywood** | 4'×8' sheet | 1 | **$70-85** | THE premium choice — no voids, no patches, all birch plies. Rip into 2× 24"×48" tops |
| **1×4 Poplar boards** (frame) | 8' length | 4 | **$24-32** | Hardwood, warp-resistant. Cut to: 4× 48" + 4× 21" |
| **1×4 Poplar boards** (legs) | 8' length | 2 | **$12-16** | Cut to: 4× 11-1/2" legs |
| **1×4 Poplar** (center brace) | 8' length | 1 | **$6-8** | Rigidity cross brace, 2× 21" |
| **Titebond III wood glue** | 8oz bottle | 1 | **$7** | Waterproof — essential for outdoor boards |
| **1-1/4" pocket hole screws** | Box of 50 | 1 | **$6** | Kreg fine-thread for hardwood |
| **1/4" × 2" carriage bolts** | For leg pivots | 4 | **$4** | Legs fold for transport |
| **1/4" wing nuts + washers** | | 4+8 | **$3** | Hand-tighten leg locks |
| **Sandpaper** | 150, 220, 320, 400 grit | Assorted | **$8** | Progressive sanding for smooth finish |
| **Non-skid rubber pads** | Adhesive-backed | 8 | **$5** | Bottom of frame — prevents sliding |

## EPOXY & DIFFUSER (LED Embedding)

| Part | Spec | Qty | Amazon | AliExpress | Notes |
|------|------|-----|--------|------------|-------|
| **TotalBoat Table Top Epoxy** | Crystal clear, 1:1 ratio, UV-resistant | 1 quart kit | **$30-35** | — | Self-leveling, crystal clear, won't yellow. Enough for all LED channels on both boards |
| **OR** Superclear Table Top Epoxy | Anti-yellow, UV resistant | 1 quart kit | **$25-30** | — | Budget alternative, also great |
| **Translucent white PETG sheet** | 1mm thick, 12"×12" | 2 sheets | **$8-12** | **$4-6** | Cut into strips for LED channel diffusers. PETG diffuses light evenly across segments |
| **OR Translucent white acrylic** | 1/16" (1.5mm) thick | 2 sheets | **$10-15** | **$5-8** | Alternative to PETG — slightly more rigid, also great diffusion |
| **Mixing cups + stir sticks** | Graduated, disposable | 1 pack | **$6** | — | For epoxy mixing — get graduated cups for accurate 1:1 ratio |
| **Nitrile gloves** | Disposable | 1 box | **$8** | — | Epoxy is messy |
| **Heat gun or butane torch** | For popping epoxy bubbles | 1 | **$12-15** | — | Quick pass over wet epoxy pops surface bubbles. A cheap kitchen torch works great |

**Epoxy process:** Route channels → lay LED strips → place diffuser strips → pour clear epoxy → let cure 24-48h → sand flush (320→400→600 grit) → apply spar urethane over everything

## FINISH

| Part | Spec | Qty | Est. Cost | Notes |
|------|------|-----|-----------|-------|
| **Varathane Water-Based Exterior Spar Urethane** | Satin finish | 1 quart | **$18** | The pro choice — UV protection, won't yellow, self-leveling. 5-7 coats on playing surface |
| **Foam brushes (2")** | Disposable | 6-pack | **$4** | Use these NOT rollers (rollers create bubbles) |
| **Tack cloth** | Lint-free | 2-pack | **$4** | Wipe between sanding coats — removes dust |
| **Wood stain** (optional) | Any color | 1 can | **$10** | Only if staining before clear coat |

---

## WEATHERPROOFING / PROTECTION

| Part | Spec | Qty | Amazon | AliExpress | Notes |
|------|------|-----|--------|------------|-------|
| Conformal Coating Spray | Silicone-based | 1 can | **$12** | **$6** | Spray on ESP32 and solder joints — moisture protection |
| Clear Silicone Sealant | Outdoor rated | 1 tube | **$6** | **$3** | Seal wire entry points |
| IP65 Cable Glands | PG7 (3-6mm) | 4 | **$5** | **$2** | Where wires enter the ESP32 enclosure |

---

## TOOLS (if you don't already have them)

| Tool | Amazon | Notes |
|------|--------|-------|
| Soldering Iron (adjustable temp) | $25-40 | If you need one. Pinecil V2 is great and USB-C powered |
| Solder (60/40 rosin core) | $8 | |
| Wire Strippers | $10 | |
| Multimeter | $15-25 | For debugging |
| Hot Glue Gun | $10 | Quick mounting |
| Breadboard + jumpers | $8 | For prototyping |

---

## COST SUMMARY — 1 Set (2 Boards)

### Prototype / Personal Build

| Category | Amazon | AliExpress |
|----------|--------|------------|
| ESP32-S3 boards (×2) | $20 | $10 |
| LED strips (2× 5m WS2812B) | $28 | $14 |
| Buttons (red, blue, reset ×2 each) | $14 | $5 |
| Passive components (resistors, caps) | $5 | $2 |
| Connectors & wiring | $32 | $12 |
| Power banks (×2) | $26 | $16 |
| Buzzer modules (×2) | $6 | $2 |
| 3D print filament | $20 | $12 |
| Weatherproofing | $23 | $11 |
| **Electronics Subtotal** | **$174** | **$84** |
| Premium board build (Baltic Birch + Poplar) | $155 | — |
| Epoxy + diffuser materials | $55 | $35 |
| Finish (spar urethane + supplies) | $26 | — |
| **Board Build Subtotal** | **$236** | — |
| **GRAND TOTAL (premium build)** | **~$410** | — |
| **GRAND TOTAL (AliExpress electronics + build)** | **~$355** | — |

### Resale Build (AliExpress Bulk, 10+ Units)

| Category | Per Set (est.) |
|----------|---------------|
| ESP32-S3 (×2) | $8 |
| LED strips | $10 |
| Buttons | $3 |
| Components + wiring | $8 |
| Power banks (×2) | $12 |
| Buzzers | $1 |
| 3D print filament | $5 |
| Weatherproofing materials | $4 |
| **Electronics per set** | **~$51** |
| Boards (build in bulk) | $30 |
| **Total per set** | **~$81** |
| **Potential sell price** | **$200-300** |
| **Margin** | **~$120-220** |

---

## SHOPPING LIST — Phase 1 (Prototype / Dev)

Since you already have ESP32s, here's what to order now for Amazon fast ship:

**Order immediately (Amazon):**
- [ ] 2× 5m WS2812B 60LED/m strip (BTF-LIGHTING, black PCB, IP30) — ~$28
- [ ] 1× EG STARTS 5-pack 30mm LED arcade buttons (5V) — ~$8
- [ ] 1× Tactile button assortment pack — ~$5
- [ ] 1× JST SM 3-pin connector pack (20 pairs) — ~$8
- [ ] 1× 22AWG silicone wire kit (6 colors) — ~$12
- [ ] 1× Capacitor assortment (includes 1000µF) — ~$10
- [ ] 1× Resistor assortment (includes 330Ω, 10KΩ) — ~$6
- [ ] 1× Translucent white PETG filament 1kg — ~$20
- [ ] 2× Passive buzzer module — ~$6

**Phase 1 Amazon total: ~$103**

**Order now on AliExpress (for future builds / bulk):**
- [ ] 10× ESP32-S3-DevKitC N16R8 — ~$50
- [ ] 10× 5m WS2812B strips — ~$65
- [ ] 20× 30mm LED arcade buttons (red + blue) — ~$20
- [ ] Bulk JST connectors, wire, components — ~$30

**AliExpress bulk total: ~$165** (enough for 5 sets)

---

## AMAZON LINKS (Quick Order)

**LED Strips:**
- BTF-LIGHTING WS2812B 5m 60LED/m: <https://www.amazon.com/s?k=BTF-LIGHTING+WS2812B+60+5m+IP30>
- Aclorol WS2812B 5m: <https://www.amazon.com/dp/B07BH37JGC>

**Buttons:**
- EG STARTS 5-pack 30mm LED: <https://www.amazon.com/dp/B01N11BDX9>
- EG STARTS 5-pack full color: <https://www.amazon.com/dp/B06Y5Z5Q1N>

**ESP32-S3 (for final build):**
- 2-pack ESP32-S3-DevKitC N16R8: <https://www.amazon.com/dp/B0FPLSQZWF>

**Connectors:**
- BTF-LIGHTING JST SM 3-pin 20-pack: <https://www.amazon.com/dp/B01DC0KIT2>

**Wire:**
- TUOFENG 22AWG 6-color: <https://www.amazon.com/dp/B07G2JWYDW>

**Filament:**
- IEMAI Translucent White PETG: <https://www.amazon.com/dp/B0FNVGWFWG>

**Power:**
- Anker PowerCore 10000 USB-C: <https://www.amazon.com/s?k=anker+powercore+10000+usb-c>

---

*Last updated: 2026-03-05 by Bob 🔨*
