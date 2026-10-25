/* Copyright (c) 2026 Klumzy Duk Gamez.
 * See LICENSE.md for full terms. */

#include "mc_state.h"
#include "mc_window.h"
#include "mc_shader.h"
#include "mc_assets.h"

bool mc_init_state(mc_state* state) {
	if (!mc_init_window(&state->window, &state->glContext)) {
		SDL_Log("Failed to init window.");
		return false;
	}

	if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress)) {
		SDL_Log("Failed to load GL.");
		return false;
	}

	mc_init_camera(&state->camera);

	if (!mc_compile_shader(mc_asset_test_vert, mc_asset_test_frag, &state->testProgram)) {
		SDL_Log("Failed to compile shader (mc_test.vert, mc_test.frag)");
		return false;
	}

	glGenVertexArrays(1, &state->vao);

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
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	static float time = 0;
	time++;
	state->camera.targetFov = 90 + (SDL_sinf(time * 0.06)) * 50;

	mc_calculate_camera(&state->camera);
	SDL_Log("%f", state->camera.targetFov);

	glBindVertexArray(state->vao);
	glUseProgram(state->testProgram);
	glUniformMatrix4fv(glGetUniformLocation(state->testProgram, "uProjectionView"), 1, GL_FALSE, (const GLfloat*)state->camera.projectionView);
	glDrawArrays(GL_TRIANGLES, 0, 3);

	if (!SDL_GL_SwapWindow(state->window)) {
		SDL_Log("Failed to swap window buffers.");
	}
	return true;
}

void mc_cleanup_state(mc_state* state) {
	glDeleteVertexArrays(1, &state->vao);
	glDeleteProgram(state->testProgram);
	mc_cleanup_window(&state->window, &state->glContext);
}
