#!/bin/bash

#!/usr/bin/env bash
set -euo pipefail

# Maps to benchmark (edit as needed)
MAPS=(BAY NY COL FLA NW NE)

# Test cases: "MAP START GOAL"
# Fill these with known-valid vertex IDs for each map.
# Example includes the known BAY case from the statement.
CASES=(
  "BAY 58370 58294"
  "BAY 286447 114579"
  "BAY 222904 98752"
  "NY 58370 58294"
  "NY 38903 179206"
  "NY 89185 4238"
  "COL 335244 335279"
  "COL 334467 122074"
  "COL 27521 315897"
  "FLA 233479 239259"
  "FLA 643455 617648"
  "FLA 267069 247309"
  "NW 233479 791668"
  "NW 1068547 395621"
  "NW 821562 1061861"
  "NE 1340976 1341114"
  "NE 1458835 1267522"
  "NE 1475910 342553"
)

ALGS=(astar dijk bfs dfs)

mkdir -p bench_out bench_logs

run_one_alg() {
  local alg="$1"
  local exe="./parte2-${alg}"

  if [[ ! -x "${exe}" ]]; then
    echo "ERROR: missing executable ${exe}"
    exit 1
  fi

  echo "=== Running algorithm: ${alg} ==="

  # Make parte-2.py use the selected binary
  ln -sf "${exe}" ./parte2

  for tc in "${CASES[@]}"; do
    read -r MAP S T <<< "${tc}"

    local base="USA-road-d.${MAP}"
    local out="bench_out/${alg}-${MAP}-${S}-${T}.path"
    local log="bench_logs/${alg}-${MAP}-${S}-${T}.log"

    echo "--- ${alg} ${MAP} ${S}->${T} ---"
    ./parte-2.py "${S}" "${T}" "${base}" "${out}" | tee "${log}"
  done
}

# Run all test cases for one algorithm, then next algorithm, etc.
for alg in "${ALGS[@]}"; do
  run_one_alg "${alg}"
done

echo "Done. Outputs in bench_out/, logs in bench_logs/."

