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

## Raw capture attempt in libogc

Best current low-scope probe path:

- Use `WPAD_Data(0)` and inspect `WPADData.exp` when `data_present` includes `WPAD_DATA_EXPANSION`.
- If `exp.type == WPAD_EXP_UNKNOWN`, copy the first 6 bytes of the `exp` storage as the raw report payload.

Reasoning:

- `WPADData` always carries an `expansion_t exp` field in libogc headers.
- For known extensions, libogc overlays typed structs on that storage.
- For an unknown third-party device like uDraw, the most plausible minimal probe strategy is that the same storage still holds the raw 6-byte payload.

This is intentionally a probe hypothesis, not a claim of confirmed behavior. Real Wii hardware verification is still required.

## libogc status on current host

Current host does not appear to have a local devkitPro/libogc tree installed under `/opt/devkitpro`, so local compile verification is blocked until toolchain is present.
