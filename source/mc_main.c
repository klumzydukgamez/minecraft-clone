/* Copyright (c) 2026 Klumzy Duk Gamez.
 * See LICENSE.md for full terms. */

#include "mc_third_party.h"
#include "mc_state.h"

#define SDL_MAIN_USE_CALLBACKS 1
#include <SDL3/SDL_main.h>

SDL_AppResult SDL_AppInit(void** appstate, int argc, char** argv) {
	*appstate = SDL_malloc(sizeof(mc_state));
	if (!*appstate) {
		return SDL_APP_FAILURE;
	}
	mc_state* state = (mc_state*)*appstate;
	if (!mc_init_state(state)) {
		return SDL_APP_FAILURE;
	}
	return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppEvent(void* appstate, SDL_Event* event) {
	if (!appstate) {
		return SDL_APP_FAILURE;
	}
	mc_state* state = (mc_state*)appstate;
	if (!mc_poll_state(state, event)) {
		return SDL_APP_SUCCESS;
	}
	return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppIterate(void* appstate) {
	if (!appstate) {
		return SDL_APP_FAILURE;
	}
	mc_state* state = (mc_state*)appstate;
	if (!mc_iterate_state(state)) {
		return SDL_APP_SUCCESS;
	}
	return SDL_APP_CONTINUE;
}

void SDL_AppQuit(void* appstate, SDL_AppResult result) {
	if (!appstate) {
		return;
	}
	mc_state* state = (mc_state*)appstate;
	mc_cleanup_state(state);
	SDL_free(appstate);
}
