# Research notes

## Initial uDraw decode model

Using Dolphin's uDraw emulation implementation as a reference model for the 6-byte report layout and neutral values.

Observed model:
- Extension ID: `ff 00 a4 20 01 12`
- 6-byte data layout:
  - byte 0: stylus X low 8 bits
  - byte 1: stylus Y low 8 bits
  - byte 2: stylus X high nibble in low 4 bits, stylus Y high nibble in high 4 bits
  - byte 3: pressure
  - byte 4: constant `0xff`
  - byte 5: buttons, active low, neutral `0xfb`
- Lifted stylus state: bytes 0-2 all `0xff`
- Buttons:
  - bit 0 clear = upper rocker/button pressed
  - bit 1 clear = lower rocker/button pressed

Assumption for first probe build:
- Real hardware raw report layout matches the Dolphin model closely enough to be a useful starting point.
- If raw bytes disagree on console, keep raw display authoritative and adjust decode after hardware capture.

## libogc status on current host

Current host does not appear to have a local devkitPro/libogc tree installed under `/opt/devkitpro`, so local compile verification may be blocked until toolchain is present.
