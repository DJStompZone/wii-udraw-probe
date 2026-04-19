#!/usr/bin/env bash
set -euo pipefail

ROOT="$(cd "$(dirname "$0")/.." && pwd)"
cd "$ROOT"

if [[ -f /etc/profile.d/devkit-env.sh ]]; then
  # shellcheck disable=SC1091
  source /etc/profile.d/devkit-env.sh
fi

: "${DEVKITPRO:?DEVKITPRO is not set}"
: "${DEVKITPPC:?DEVKITPPC is not set}"

make clean
make
