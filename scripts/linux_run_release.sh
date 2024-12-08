#!/bin/bash
cd "$(dirname "$0")"
cd ..
./scripts/linux_build_release.sh
./build/bin/HeronTriangle