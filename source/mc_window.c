/* Copyright (c) 2026 Klumzy Duk Gamez.
 * See LICENSE.md for full terms. */

#include "mc_window.h"
#include "mc_constants.h"

bool mc_init_window(SDL_Window** window, SDL_GLContext* glContext) {
	if (!SDL_InitSubSystem(SDL_INIT_VIDEO)) {
		SDL_Log("Failed to init SDL video subsystem.");
		return false;
	}

	if (!SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3)) {
		SDL_Log("Failed to set GL attribute.");
		return false;
	}
	if (!SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3)) {
		SDL_Log("Failed to set GL attribute.");
		return false;
	}
	if (!SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE)) {
		SDL_Log("Failed to set GL attribute.");
		return false;
	}
	if (!SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1)) {
		SDL_Log("Failed to set GL attribute.");
		return false;
	}
	if (!SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 4)) {
		SDL_Log("Failed to set GL attribute.");
		return false;
	}

	*window = SDL_CreateWindow("minecraft-clone", MC_WINDOW_WIDTH, MC_WINDOW_HEIGHT, SDL_WINDOW_OPENGL);
	if (!*window) {
		SDL_Log("Failed to create SDL window.");
		return false;
	}

	if (!SDL_SetWindowRelativeMouseMode(*window, true)) {
		SDL_Log("Failed to make SDL window relative mouse mode.");
		return false;
	}

	*glContext = SDL_GL_CreateContext(*window);
	if (!*glContext) {
		SDL_Log("Failed to create SDL GL context.");
		return false;
	}

	if (!SDL_GL_MakeCurrent(*window, *glContext)) {
		SDL_Log("Failed to use SDL GL context.");
		return false;
	}

	return true;
}

void mc_cleanup_window(SDL_Window** window, SDL_GLContext* glContext) {
	if (*glContext) {
		SDL_GL_DestroyContext(*glContext);
		*glContext = nullptr;
	}
	if (*window) {
		SDL_DestroyWindow(*window);
		*window = nullptr;
	}
	SDL_QuitSubSystem(SDL_INIT_VIDEO);
}
