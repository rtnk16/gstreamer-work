#!/bin/bash
set -euo pipefail
cd -- "$(dirname -- "${BASH_SOURCE[0]}")"
if [[ -f /workspace/.gstreamer-env/activate.sh ]]; then
    source /workspace/.gstreamer-env/activate.sh
fi
if ! pkg-config --exists gstreamer-1.0; then
    echo 'GStreamer development files are required (libgstreamer1.0-dev).' >&2
    exit 1
fi
binary=$(mktemp /tmp/gstreamer-demo.XXXXXX)
trap 'rm -f -- "$binary"' EXIT
cc -Wall -Wextra -Werror main.c -o "$binary" $(pkg-config --cflags --libs gstreamer-1.0)
"$binary" "$@"
