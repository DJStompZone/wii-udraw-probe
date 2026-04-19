#!/usr/bin/env bash
set -euo pipefail

ROOT="$(cd "$(dirname "$0")/.." && pwd)"
APP_DIR="$ROOT/dist/apps/udraw-probe"
ZIP_PATH="$ROOT/dist/udraw-probe.zip"

cd "$ROOT"

"$ROOT/scripts/build.sh"

mkdir -p "$APP_DIR"
rm -f "$APP_DIR/boot.dol" "$APP_DIR/udraw_probe.dol"
cp -f "$ROOT/udraw_probe.dol" "$APP_DIR/boot.dol"

if [[ ! -f "$APP_DIR/meta.xml" ]]; then
  cat > "$APP_DIR/meta.xml" <<'XML'
<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
<app version="1">
  <name>uDraw Probe</name>
  <coder>djstomp</coder>
  <version>0.0.1</version>
  <release_date>20260418224322</release_date>
  <short_description>Probe app for the THQ uDraw GameTablet on Wii.</short_description>
  <long_description>Shows raw extension bytes plus decoded pen X/Y/pressure, button state, and min/max ranges.

Controls:
A: start/stop capture
B: save captured samples to sd:/udraw_probe_capture.txt
1: clear capture buffer and observed min/max
HOME: exit</long_description>
  <no_ios_reload />
</app>
XML
fi

if command -v python3 >/dev/null 2>&1; then
  python3 - <<'PY'
from pathlib import Path
p = Path('dist/apps/udraw-probe/meta.xml')
text = p.read_text()
text = text.replace('<name>udraw-probe</name>', '<name>uDraw Probe</name>')
text = text.replace('Reverse-engineering the Wii uDraw tablet into a proper homebrew input device.', 'Probe app for the THQ uDraw GameTablet on Wii.')
text = text.replace('save captured samples', 'save captured samples to sd:/udraw_probe_capture.txt')
p.write_text(text)
PY
fi

rm -f "$ZIP_PATH"
(
  cd "$ROOT/dist"
  zip -r "$(basename "$ZIP_PATH")" apps >/dev/null
)

echo "Built bundle: $ZIP_PATH"
echo "Homebrew app dir: $APP_DIR"
