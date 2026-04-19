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

This repository currently contains a buildable probe UI scaffold and a modular decode layer for the expected uDraw 6-byte report format. The raw device read path is not wired yet, so the UI currently uses placeholder report bytes while waiting for hardware-facing extension access work.
