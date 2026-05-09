#!/usr/bin/env bash
# Build script for Entropy Piano Tuner (Linux).
# Mirrors the flow of build_project.bat / build_qtmidi.bat:
#   1. Build the qtmidi dependency (in-tree, so libqtmidi.a lands where
#      entropypianotuner_func.pri expects it: qtmidi/qtmidi/lib/).
#   2. Run qmake + make on the top-level project.
#
# Usage:
#   ./build.sh              # build
#   ./build.sh -c           # clean first, then build
#   ./build.sh --clean-only # clean and exit
#   JOBS=4 ./build.sh       # override parallel jobs (default: nproc)
#   QMAKE=/path/qmake6 ./build.sh
#   SPEC=linux-clang ./build.sh

set -euo pipefail

ROOT_DIR="$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" && pwd)"
cd "$ROOT_DIR"

QMAKE="${QMAKE:-qmake6}"
command -v "$QMAKE" >/dev/null 2>&1 || QMAKE="qmake"
command -v "$QMAKE" >/dev/null 2>&1 || {
    echo "error: qmake not found in PATH (set QMAKE=/path/to/qmake6)" >&2
    exit 1
}

SPEC="${SPEC:-linux-g++}"
JOBS="${JOBS:-$(nproc 2>/dev/null || echo 2)}"
MAKE_ARGS=(-j"$JOBS")

CLEAN=0
CLEAN_ONLY=0
for arg in "$@"; do
    case "$arg" in
        -c|--clean) CLEAN=1 ;;
        --clean-only) CLEAN=1; CLEAN_ONLY=1 ;;
        -h|--help)
            sed -n '2,16p' "$0"
            exit 0
            ;;
        *) echo "error: unknown argument: $arg" >&2; exit 2 ;;
    esac
done

clean_tree() {
    echo "==> Cleaning build artifacts"
    find . -name '.qmake.stash' -delete 2>/dev/null || true
    find . -name 'Makefile' -path '*/qtmidi/*' -delete 2>/dev/null || true
    [ -f Makefile ] && make distclean >/dev/null 2>&1 || true
    [ -f qtmidi/Makefile ] && (cd qtmidi && make distclean >/dev/null 2>&1) || true
    rm -rf qtmidi/qtmidi/lib
}

if [ "$CLEAN" = 1 ]; then
    clean_tree
    [ "$CLEAN_ONLY" = 1 ] && exit 0
fi

echo "==> qmake: $($QMAKE -v | tail -n1)"
echo "==> spec: $SPEC, jobs: $JOBS"

# 1) qtmidi
echo "==> Building qtmidi"
(
    cd qtmidi
    rm -f .qmake.stash
    "$QMAKE" -spec "$SPEC"
    make "${MAKE_ARGS[@]}"
)

# 2) main project
echo "==> Building entropypianotuner"
rm -f .qmake.stash
"$QMAKE" -spec "$SPEC" entropypianotuner.pro
make "${MAKE_ARGS[@]}"

echo "==> Build finished. Binaries are under bin/"
