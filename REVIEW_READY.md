# REVIEW_READY.md

## Summary

The Wii uDraw probe project is now review-ready by the agreed definition: the software build, UI, controls, and test guidance are in place, and the only remaining meaningful step is real-hardware tryout on DJ's Wii with a real THQ uDraw GameTablet.

## What was implemented

- Buildable Wii homebrew project using devkitPPC/libogc
- Modular source layout with separate probe, decode, capture, and UI layers
- On-screen debug UI showing:
  - raw extension ID bytes
  - raw 6-byte report
  - decoded pen X
  - decoded pen Y
  - decoded pen pressure
  - upper pen button
  - lower pen button
  - pen in range / pen lifted
  - observed runtime min/max for X, Y, pressure
- Status badges and button hints to make the UI understandable on first run
- Optional in-memory capture recording
- Optional SD export to `sd:/udraw_probe_capture.txt`
- `TESTING.md` operator guide for real Wii hardware testing

## Assumptions that remain

- The initial decode model follows the WiiBrew/Dolphin uDraw 6-byte report format assumption.
- The current live-read path assumes that, on real hardware, libogc may expose unknown-extension bytes through `WPAD_Data(0)->exp` when the uDraw is attached.

## What still needs real hardware verification

- Whether the uDraw appears as `WPAD_EXP_UNKNOWN` on the target Wii/libogc stack
- Whether `WPAD_Data(0)->exp` actually carries the true raw 6-byte payload for the tablet
- Whether decoded X/Y/pressure and button states behave plausibly on real hardware
- Whether SD capture export works as expected in the user's loader/setup

## Current judgment

The software side is at the point where further purely local work is unlikely to beat a real-device test. If the live path works, great. If not, the app should still provide enough status and capture detail to guide the next revision.
