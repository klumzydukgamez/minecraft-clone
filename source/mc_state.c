/* Copyright (c) 2026 Klumzy Duk Gamez.
 * See LICENSE.md for full terms. */

#include "mc_state.h"
#include "mc_window.h"

bool mc_init_state(mc_state* state) {
	if (!mc_init_window(&state->window, &state->glContext)) {
		return false;
	}

	if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress)) {
		return false;
	}

	return true;
}

bool mc_poll_state(mc_state* state, SDL_Event* event) {
	switch (event->type) {
		case SDL_EVENT_QUIT:
			return false;
	}

	return true;
}

bool mc_iterate_state(mc_state* state) {
	glViewport(0, 0, MC_WINDOW_WIDTH, MC_WINDOW_HEIGHT);
	glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	if (!SDL_GL_SwapWindow(state->window)) {
	}
	return true;
}

void mc_cleanup_state(mc_state* state) {
	mc_cleanup_window(&state->window, &state->glContext);
}
