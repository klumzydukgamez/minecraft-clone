:: Copyright (c) 2026 Klumzy Duk Gamez.
:: See LICENSE.md for full terms.

@echo off

if not exist build mkdir build

cd build
cls

cmake .. -G Ninja
cmake --build .
cmake --install .

cd ..
pause
cls

