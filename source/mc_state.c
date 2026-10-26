/* Copyright (c) 2026 Klumzy Duk Gamez.
 * See LICENSE.md for full terms. */

#include "mc_state.h"
#include "mc_window.h"
#include "mc_shader.h"
#include "mc_texture.h"
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

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	state->lastTicks = SDL_GetTicksNS();

	if (!mc_init_asset_png(mc_asset_blocks_png, mc_asset_blocks_png_size, &state->blockTexture)) {
		SDL_Log("Failed to init asset (mc_blocks.png).");
		return false;
	}

	if (!mc_compile_shader(mc_asset_chunk_vert, mc_asset_chunk_frag, &state->chunkProgram)) {
		SDL_Log("Failed to compile shader (mc_test.vert, mc_test.frag)");
		return false;
	}
	state->chunkUniformProjectionViewLocation = glGetUniformLocation(state->chunkProgram, "uProjectionView");
	if (state->chunkUniformProjectionViewLocation == -1) {
		SDL_Log("Failed to get GL uniform location (uProjectionView).");
		return false;
	}
	state->chunkUniformTextureLocation = glGetUniformLocation(state->chunkProgram, "uTexture");
	if (state->chunkUniformTextureLocation == -1) {
		SDL_Log("Failed to get GL uniform location (uTexture).");
		return false;
	}

	if (!mc_init_chunk(&state->chunk)) {
		SDL_Log("Failed to init chunk.");
		return false;
	}

	mc_init_player(&state->player);

	return true;
}

bool mc_poll_state(mc_state* state, SDL_Event* event) {
	switch (event->type) {
		case SDL_EVENT_QUIT:
			return false;
		case SDL_EVENT_KEY_DOWN:
			switch (event->key.scancode) {
				case SDL_SCANCODE_W:
					state->playerInput.forward = true;
					break;
				case SDL_SCANCODE_S:
					state->playerInput.backward = true;
					break;
				case SDL_SCANCODE_A:
					state->playerInput.left = true;
					break;
				case SDL_SCANCODE_D:
					state->playerInput.right = true;
					break;
				case SDL_SCANCODE_SPACE:
					state->playerInput.up = true;
					break;
				case SDL_SCANCODE_LCTRL:
					state->playerInput.down = true;
					break;
				case SDL_SCANCODE_M:
					state->player.mode++;
					if (state->player.mode > MC_SPECTATOR_MODE)
						state->player.mode = MC_SURVIVAL_MODE;
					break;
				case SDL_SCANCODE_ESCAPE:
					return false;
				default:
					break;
			}
			break;
		case SDL_EVENT_KEY_UP:
			switch (event->key.scancode) {
				case SDL_SCANCODE_W:
					state->playerInput.forward = false;
					break;
				case SDL_SCANCODE_S:
					state->playerInput.backward = false;
					break;
				case SDL_SCANCODE_A:
					state->playerInput.left = false;
					break;
				case SDL_SCANCODE_D:
					state->playerInput.right = false;
					break;
				case SDL_SCANCODE_SPACE:
					state->playerInput.up = false;
					break;
				case SDL_SCANCODE_LCTRL:
					state->playerInput.down = false;
					break;
				default:
					break;
			}
			break;

		case SDL_EVENT_MOUSE_MOTION:
			state->playerInput.xrel += event->motion.xrel;
			state->playerInput.yrel += event->motion.yrel;
			break;
		case SDL_EVENT_MOUSE_BUTTON_DOWN:
			switch (event->button.button) {
				case SDL_BUTTON_LEFT:
					state->playerInput.lmb = true;
					break;
				case SDL_BUTTON_RIGHT:
					state->playerInput.rmb = true;
					break;
				default:
					break;
			}
			break;
		case SDL_EVENT_MOUSE_BUTTON_UP:
			switch (event->button.button) {
				case SDL_BUTTON_LEFT:
					state->playerInput.lmb = false;
					break;
				case SDL_BUTTON_RIGHT:
					state->playerInput.rmb = false;
					break;
				default:
					break;
			}

		default:
			break;
	}

	return true;
}

bool mc_iterate_state(mc_state* state) {
	Uint64 currentTicks = SDL_GetTicksNS();
	Uint64 elapsedTicks = currentTicks - state->lastTicks;
	state->lastTicks = currentTicks;
	if (elapsedTicks >= MC_MAX_TICKS)
		elapsedTicks = MC_MAX_TICKS;
	state->accumulatedTicks += elapsedTicks;
	state->fpsCount++;
	state->fpsTimer += elapsedTicks;
	if (state->fpsTimer >= MC_ONE_SECOND) {
		state->fps = state->fpsCount;
		state->fpsCount = 0;
		state->fpsTimer %= MC_ONE_SECOND;
		char buffer[32];
		SDL_snprintf(buffer, sizeof(buffer), "minecraft-clone | %d", state->fps);
		SDL_SetWindowTitle(state->window, buffer);
	}

	while (state->accumulatedTicks >= MC_FRAME_TICKS) {
		state->accumulatedTicks -= MC_FRAME_TICKS;
		state->ticks++;

		mc_update_player(&state->player, &state->playerInput, &state->chunk);
		state->playerInput.xrel = 0;
		state->playerInput.yrel = 0;

		if (!mc_update_chunk(&state->chunk)) {
			SDL_Log("Failed to update chunk.");
		}
	}

	glViewport(0, 0, MC_WINDOW_WIDTH, MC_WINDOW_HEIGHT);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glUseProgram(state->chunkProgram);
	glUniformMatrix4fv(state->chunkUniformProjectionViewLocation, 1, GL_FALSE, (GLfloat*)state->player.projectionView);
	glUniform1i(state->chunkUniformTextureLocation, 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, state->blockTexture);
	glBindVertexArray(state->chunk.vertexArray);
	glDrawArrays(GL_TRIANGLES, 0, state->chunk.vertexCount);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	if (!SDL_GL_SwapWindow(state->window)) {
		SDL_Log("Failed to swap window buffers.");
	}
	return true;
}

void mc_cleanup_state(mc_state* state) {
	mc_cleanup_chunk(&state->chunk);
	if (state->chunkProgram)
		glDeleteProgram(state->chunkProgram);
	if (state->blockTexture)
		glDeleteTextures(1, &state->blockTexture);
	mc_cleanup_window(&state->window, &state->glContext);
}
