#!/usr/bin/env bash
set -euo pipefail

ROOT="$(cd "$(dirname "$0")/.." && pwd)"
APP_DIR="$ROOT/dist/apps/udraw-probe"
ZIP_PATH="$ROOT/dist/udraw-probe.zip"

cd "$ROOT"

"$ROOT/scripts/build.sh"

mkdir -p "$APP_DIR"
rm -f "$APP_DIR/boot.dol" "$APP_DIR/udraw_probe.dol" "$APP_DIR/meta.xml" "$APP_DIR/icon.png"
cp -f "$ROOT/udraw_probe.dol" "$APP_DIR/boot.dol"
cp -f "$ROOT/assets/meta.xml" "$APP_DIR/meta.xml"
cp -f "$ROOT/assets/icon.png" "$APP_DIR/icon.png"

rm -f "$ZIP_PATH"
(
  cd "$ROOT/dist"
  zip -r "$(basename "$ZIP_PATH")" apps >/dev/null
)

echo "Built bundle: $ZIP_PATH"
echo "Homebrew app dir: $APP_DIR"
