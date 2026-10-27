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

constexpr float MC_BLOCK_FACE_U = 1.0f / 16.0f;
constexpr float MC_BLOCK_FACE_V = 1.0f / 16.0f;

constexpr int MC_CHUNK_WIDTH = 16;
constexpr int MC_CHUNK_HEIGHT = 256;
constexpr int MC_CHUNK_BLOCK_COUNT = MC_CHUNK_WIDTH * MC_CHUNK_WIDTH * MC_CHUNK_HEIGHT;

constexpr int MC_WORLD_CHUNK_RADIUS = 32;
constexpr int MC_WORLD_CHUNKS_WIDTH = (MC_WORLD_CHUNK_RADIUS * 2) + 1;

constexpr float MC_PLAYER_FOV = 80;
constexpr float MC_PLAYER_SENSITIVITY = 1.5f;
constexpr float MC_PLAYER_HEIGHT = 1.6f;
constexpr float MC_PLAYER_WIDTH = 0.6f;
constexpr float MC_PLAYER_HALF_WIDTH = MC_PLAYER_WIDTH / 2.0f;
constexpr float MC_PLAYER_REACH = 5.0f;

constexpr float MC_PLAYER_ACCELERATION = 0.08f; /* 0.012f */
constexpr float MC_PLAYER_MAX_SPEED = 0.12f; /* 0.07f */
constexpr float MC_PLAYER_FRICTION = 0.55f; /* 0.70f */
constexpr float MC_PLAYER_AIR_FRICTION = 0.91f;
constexpr float MC_PLAYER_JUMP_VELOCITY = 0.13f; /* 0.12f */
constexpr float MC_PLAYER_GRAVITY = -0.006f; /* -0.007f */
constexpr float MC_PLAYER_TERMINAL_VELOCITY = -0.4f; /* -0.6f */

constexpr float MC_PLAYER_CROUCH_HEIGHT = 1.1f;
constexpr float MC_PLAYER_CROUCH_ACCELERATION = 0.4f;

#endif /* MC_CONSTANTS_H */
