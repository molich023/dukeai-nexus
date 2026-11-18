#!/usr/bin/env bash
set -euo pipefail

ROOT="$(cd "$(dirname "$0")/.." && pwd)"
BUILD="$ROOT/build"

cmake -S "$ROOT" -B "$BUILD"
cmake --build "$BUILD" --parallel 2

OUTPUT="$("$BUILD/dukeai_ranker" \
    "$ROOT/tests/data/ranking_pairs.tsv")"

printf '%s\n' "$OUTPUT"

grep -q 'model=linear_energy_ranker' <<< "$OUTPUT"
grep -q 'training_pairs=6' <<< "$OUTPUT"
grep -q 'training_pair_accuracy=1.0000' <<< "$OUTPUT"
grep -q 'warning=training accuracy is not held-out accuracy' \
    <<< "$OUTPUT"

echo "PASS: ranker smoke test"
