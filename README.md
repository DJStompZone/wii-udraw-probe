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
