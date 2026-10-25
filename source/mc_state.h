/* Copyright (c) 2026 Klumzy Duk Gamez.
 * See LICENSE.md for full terms. */

#if !defined MC_STATE_H
#define MC_STATE_H

#include "mc_third_party.h"
#include "mc_constants.h"
#include "mc_player.h"
#include "mc_chunk.h"

typedef struct {
	SDL_Window* window;
	SDL_GLContext glContext;

	Uint64 lastTicks;
	Uint64 accumulatedTicks;
	Uint64 ticks;

	Uint64 fpsTimer;
	uint32_t fpsCount;
	uint32_t fps;

	mc_player_input playerInput;
	mc_player player;

	GLuint vao;
	GLuint testProgram;

	mc_chunk chunks[MC_CHUNK_RADIUS][MC_CHUNK_RADIUS];
} mc_state;

bool mc_init_state(mc_state* state);
bool mc_poll_state(mc_state* state, SDL_Event* event);
bool mc_iterate_state(mc_state* state);
void mc_cleanup_state(mc_state* state);

#endif /* MC_STATE_H */
