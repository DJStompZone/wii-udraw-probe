# Wii uDraw Probe

Minimal Wii homebrew probe app scaffold for the THQ uDraw GameTablet.

## Build

Requires devkitPro with devkitPPC and libogc.

```sh
export DEVKITPRO=/opt/devkitpro
export DEVKITPPC=$DEVKITPRO/devkitPPC
make
```

## Run

Copy `udraw_probe.dol` to your preferred loader flow, or use:

```sh
make run
```

## Current status

This repository contains a probe UI and modular decode layer for the expected uDraw 6-byte report format.

Current runtime behavior:

- If libogc exposes unknown-extension bytes through `WPAD_Data(0)->exp`, the app copies those live 6 bytes into the on-screen raw report view and decodes them.
- If libogc does not expose those bytes on the current stack, the app clearly reports that and falls back to placeholder bytes so the UI and decode path remain visible.

That means the remaining uncertainty is now focused on real-hardware verification: whether `WPADData.exp` for `WPAD_EXP_UNKNOWN` actually contains the raw uDraw payload on console.
