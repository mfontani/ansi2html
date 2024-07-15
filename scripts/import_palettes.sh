#!/bin/bash

set -e

if [[ ! -d iTerm2-Color-Schemes ]]; then
    git clone https://github.com/mbadolato/iTerm2-Color-Schemes/
fi

(
    cd iTerm2-Color-Schemes
    git pull
)

mkdir -p src/iterm2_color_schemes/
./scripts/iterm2-color-schemes.pl "$(git -C iTerm2-Color-Schemes rev-parse HEAD)"
