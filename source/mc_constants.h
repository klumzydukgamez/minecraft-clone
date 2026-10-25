/* Copyright (c) 2026 Klumzy Duk Gamez.
 * See LICENSE.md for full terms. */

#if !defined MC_CONSTANTS_H
#define MC_CONSTANTS_H

#include "mc_third_party.h"

constexpr int MC_WINDOW_WIDTH = 1280;
constexpr int MC_WINDOW_HEIGHT = 720;
constexpr float MC_WINDOW_RATIO = (float)MC_WINDOW_WIDTH / (float)MC_WINDOW_HEIGHT;

constexpr Uint64 MC_MAX_TICKS = 250'000'000;
constexpr Uint64 MC_ONE_SECOND = 1'000'000'000;
constexpr Uint64 MC_FRAME_TICKS = 16'666'666;

constexpr float MC_PLAYER_FOV = 80;
constexpr float MC_PLAYER_SENSITIVITY = 1.5f;
constexpr float MC_PLAYER_HEIGHT = 1.5f;

constexpr float MC_PLAYER_ACCELERATION = 0.2f;
constexpr float MC_PLAYER_MAX_SPEED = 5.0f;
constexpr float MC_PLAYER_FRICTION = 0.1f;

constexpr size_t MC_CHUNK_WIDTH = 16;
constexpr size_t MC_CHUNK_HEIGHT = 16;

constexpr size_t MC_CHUNK_RADIUS = 16;

#endif /* MC_CONSTANTS_H */
