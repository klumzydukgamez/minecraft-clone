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

	mc_init_camera(&state->camera);

	return true;
}

bool mc_poll_state(mc_state* state, SDL_Event* event) {
	switch (event->type) {
		case SDL_EVENT_QUIT:
			return false;
		case SDL_EVENT_MOUSE_MOTION:
			mc_update_camera(&state->camera, event->motion.xrel, event->motion.yrel);
			break;
	}

	return true;
}

bool mc_iterate_state(mc_state* state) {
	glViewport(0, 0, MC_WINDOW_WIDTH, MC_WINDOW_HEIGHT);
	glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	mc_calculate_camera(&state->camera);
	SDL_Log("%f, %f, %f", state->camera.front[0], state->camera.front[1], state->camera.front[2]);

	if (!SDL_GL_SwapWindow(state->window)) {
	}
	return true;
}

void mc_cleanup_state(mc_state* state) {
	mc_cleanup_window(&state->window, &state->glContext);
}
