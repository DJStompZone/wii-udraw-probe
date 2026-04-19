# Wii uDraw Probe Test Guide

This is the short operator guide for trying the probe on a real Wii with a real THQ uDraw GameTablet.

## What you need

- A Wii capable of running homebrew
- A Wiimote on channel 1 / player 1
- A real THQ uDraw GameTablet connected as the Wiimote extension
- An SD card if you want capture export

## Launching

Load `udraw_probe.dol` with your usual homebrew loader.

## What to look for

### Best case

You should see:
- `STATUS [LIVE]`
- a non-placeholder raw 6-byte report changing as you move the pen
- changing decoded X/Y/pressure values
- button changes when pressing the tablet buttons
- min/max values updating while you move the pen around

### Still useful, but not ideal

You may see:
- `STATUS [WAIT]` meaning the extension is present but raw unknown-extension bytes are not exposed through the current libogc path
- `STATUS [FALLBACK]` meaning the UI is alive but showing scaffold bytes rather than true hardware bytes

If that happens, please still report exactly what the screen says.

## Controls

- `A`: start/stop capture
- `B`: save capture to `sd:/udraw_probe_capture.txt`
- `1`: clear capture buffer and reset observed min/max
- `HOME`: exit

## Suggested first test

1. Launch the app with the tablet connected.
2. Check the status badge.
3. Move the pen around and watch raw bytes plus decoded X/Y/pressure.
4. Tap both tablet buttons.
5. Lift and reintroduce the pen.
6. Press `A` to record a short sample.
7. Draw or move for a few seconds.
8. Press `A` again to stop.
9. Press `B` to save the capture file.

## What to send back

If you test it, the most useful feedback will be:
- the status badge shown on screen (`LIVE`, `WAIT`, `FALLBACK`, or `CHECK`)
- whether raw bytes change with pen movement
- whether decoded X/Y/pressure look plausible
- whether button states change correctly
- whether `sd:/udraw_probe_capture.txt` was created
- a photo of the screen, if convenient
- the capture file contents, if available
