/* Copyright (c) 2026 Klumzy Duk Gamez.
 * See LICENSE.md for full terms. */

#include "mc_player.h"
#include "mc_constants.h"

void mc_init_player(mc_player* player) {
	glm_vec3_copy((vec3){0.0f, 0.0f, 1.0f}, player->position);
	glm_vec3_zero(player->velocity);

	player->fov = MC_PLAYER_FOV;
	player->targetFov = player->fov;

	player->sensitivity = MC_PLAYER_SENSITIVITY;
	player->targetSensitivity = player->sensitivity;

	player->height = MC_PLAYER_HEIGHT;
	player->targetHeight = player->height;

	player->yaw = -90.0f;
	player->pitch = 0.0f;

	glm_vec3_copy((vec3){0.0f, 0.0f, -1.0f}, player->front);
	glm_vec3_copy((vec3){1.0f, 0.0f, 0.0f}, player->right);
	glm_vec3_copy((vec3){0.0f, 1.0f, 0.0f}, player->up);

	glm_mat4_identity(player->view);
	glm_mat4_identity(player->projection);
	glm_mat4_identity(player->projectionView);
}

void mc_update_player(mc_player* player, mc_player_input* input) {
	if (SDL_fabsf(player->sensitivity - player->targetSensitivity) >= 0.001f)
		player->sensitivity = glm_lerp(player->sensitivity, player->targetSensitivity, 0.25f);
	else
		player->sensitivity = player->targetSensitivity;

	float f = player->sensitivity * 0.6f + 0.2f;
	float sensitivity = f * f * f * 0.15f;
	player->yaw += input->xrel * sensitivity;
	player->pitch += -input->yrel * sensitivity;
	if (player->pitch >= 89.9f)
		player->pitch = 89.9f;
	if (player->pitch <= -89.9f)
		player->pitch = -89.9f;

	float yaw = glm_rad(player->yaw);
	float pitch = glm_rad(player->pitch);

	vec3 front;
	front[0] = SDL_cosf(yaw) * SDL_cosf(pitch);
	front[1] = SDL_sinf(pitch);
	front[2] = SDL_sinf(yaw) * SDL_cosf(pitch);
	glm_vec3_normalize_to(front, player->front);

	vec3 up = {0.0f, 1.0f, 0.0f};

	glm_vec3_cross(player->front, up, player->right);
	glm_vec3_normalize(player->right);

	glm_vec3_cross(player->right, player->front, player->up);
	glm_vec3_normalize(player->up);

	vec3 flatFront = {player->front[0], 0.0f, player->front[2]};
	vec3 flatRight = {player->right[0], 0.0f, player->right[2]};
	if (glm_vec3_norm2(flatFront) > 0.0f)
		glm_vec3_normalize(flatFront);
	if (glm_vec3_norm2(flatRight) > 0.0f)
		glm_vec3_normalize(flatRight);

	player->velocity[0] *= MC_PLAYER_FRICTION;
	player->velocity[2] *= MC_PLAYER_FRICTION;

	vec3 direction;
	glm_vec3_zero(direction);
	if (input->forward)
		glm_vec3_add(direction, flatFront, direction);
	if (input->backward)
		glm_vec3_sub(direction, flatFront, direction);
	if (input->left)
		glm_vec3_sub(direction, flatRight, direction);
	if (input->right)
		glm_vec3_add(direction, flatRight, direction);

	if (glm_vec3_norm2(direction) > 0.0f) {
		glm_vec3_normalize(direction);
		player->velocity[0] += direction[0] * MC_PLAYER_ACCELERATION;
		player->velocity[2] += direction[2] * MC_PLAYER_ACCELERATION;
		float length = SDL_sqrtf(player->velocity[0] * player->velocity[0] + player->velocity[2] * player->velocity[2]);
		if (length > MC_PLAYER_MAX_SPEED) {
			float scale = MC_PLAYER_MAX_SPEED / length;
			player->velocity[0] *= scale;
			player->velocity[1] *= scale;
		}
	} else {
		if (SDL_fabsf((player->velocity[0]) < 0.01f))
			player->velocity[0] = 0.0f;
		if (SDL_fabsf(player->velocity[2]) < 0.01f)
			player->velocity[2] = 0.0f;
	}

	glm_vec3_add(player->position, player->velocity, player->position);

	if (SDL_fabsf(player->height - player->targetHeight) >= 0.01f)
		player->height = glm_lerp(player->height, player->targetHeight, 0.25f);
	else
		player->height = player->targetHeight;

	vec3 eye;
	glm_vec3_add(player->position, (vec3){0.0f, player->height, 0.0f}, eye);

	vec3 view;
	glm_vec3_add(eye, player->front, view);
	glm_lookat(eye, view, player->up, player->view);

	if (SDL_fabsf(player->fov - player->targetFov) >= 0.01f)
		player->fov = glm_lerp(player->fov, player->targetFov, 0.25f);
	else
		player->fov = player->targetFov;

	glm_perspective(glm_rad(player->fov), MC_WINDOW_RATIO, 0.01f, 100.0f, player->projection);
	glm_mat4_mul(player->projection, player->view, player->projectionView);
}
