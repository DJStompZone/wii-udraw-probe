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

This repository builds successfully into `udraw_probe.dol` and contains a probe UI and modular decode layer for the expected uDraw 6-byte report format.

Current runtime behavior:

- If libogc exposes unknown-extension bytes through `WPAD_Data(0)->exp`, the app copies those live 6 bytes into the on-screen raw report view and decodes them.
- If libogc does not expose those bytes on the current stack, the app clearly reports that and falls back to placeholder bytes so the UI and decode path remain visible.

That means the remaining uncertainty is now focused on real-hardware verification: whether `WPADData.exp` for `WPAD_EXP_UNKNOWN` actually contains the raw uDraw payload on console.


## Controls

- `A`: start/stop capture
- `B`: save captured samples to `sd:/udraw_probe_capture.txt`
- `1`: clear capture buffer and observed min/max
- `HOME`: exit

## Optional SD capture

When capture is enabled, the app records up to 512 samples of raw report bytes plus decoded fields in memory. Press `B` to write them to `sd:/udraw_probe_capture.txt` for later inspection.


## Real hardware test guide

See `TESTING.md` for the short operator guide and what to report back after trying the app on a real Wii + uDraw tablet.


## Helper scripts

- `scripts/build.sh`: clean rebuild using the devkitPro environment
- `scripts/bundle.sh`: rebuild, copy the app to `dist/apps/udraw-probe/boot.dol`, and create `dist/udraw-probe.zip`


## GitHub Actions

A workflow is included at `.github/workflows/build.yml` to build and bundle the app on pushes, pull requests, and manual runs.
