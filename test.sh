#!/usr/bin/env bash
# Build and run the assertion tests: ./test.sh
# Compiles test/test_main.cpp (C++17, no dependencies) into a temp dir,
# runs it, and reports the check count. Exits non-zero on any failure.
set -e
cd "$(dirname "$0")"

tmpdir="$(mktemp -d)"
trap 'rm -rf "$tmpdir"' EXIT

g++ -std=c++17 -Wall -Wextra -O2 -o "$tmpdir/run-tests" test/test_main.cpp
"$tmpdir/run-tests"
