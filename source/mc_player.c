/* Copyright (c) 2026 Klumzy Duk Gamez.
 * See LICENSE.md for full terms. */

#include "mc_player.h"
#include "mc_constants.h"

void mc_init_player(mc_player* player) {
	glm_vec3_copy((vec3){MC_CHUNK_WIDTH / 2.0f, MC_CHUNK_HEIGHT / 3.0f, MC_CHUNK_WIDTH / 2.0f}, player->position);
	glm_vec3_zero(player->velocity);
	player->mode = MC_SPECTATOR_MODE;
	player->block = MC_DIRT_BLOCK;
	player->crouched = false;
	player->grounded = false;

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

bool mc_collide_player(mc_player* player, mc_chunk* chunk) {
	int mnx = (int)SDL_floorf(player->position[0] - MC_PLAYER_HALF_WIDTH);
	int mxx = (int)SDL_floorf(player->position[0] + MC_PLAYER_HALF_WIDTH);
	int mny = (int)SDL_floorf(player->position[1]);
	int mxy = (int)SDL_floorf(player->position[1] + player->height);
	int mnz = (int)SDL_floorf(player->position[2] - MC_PLAYER_HALF_WIDTH);
	int mxz = (int)SDL_floorf(player->position[2] + MC_PLAYER_HALF_WIDTH);

	for (int y = mny; y <= mxy; y++) {
		for (int z = mnz; z <= mxz; z++) {
			int lz = z & (MC_WINDOW_WIDTH - 1);
			for (int x = mnx; x <= mxx; x++) {
				int lx = x & (MC_WINDOW_WIDTH - 1);
				mc_block block = chunk->blocks[mc_block_index(lx, y, lz)];
				if (block != MC_AIR_BLOCK) {
					if (player->position[0] + MC_PLAYER_HALF_WIDTH > (float)x && player->position[0] - MC_PLAYER_HALF_WIDTH < (float)x + 1.0f &&
						player->position[1] + player->height > (float)y && player->position[1] < (float)y + 1.0f &&
						player->position[2] + MC_PLAYER_HALF_WIDTH > (float)z && player->position[2] - MC_PLAYER_HALF_WIDTH < (float)z + 1.0f) {
						return true;
					}
				}
			}
		}
	}
	return false;
}

void mc_raycast_player(mc_player* player, mc_player_raycast* raycast, mc_chunk* chunk) {
	raycast->hit = false;

	vec3 eye;
	glm_vec3_add(player->position, (vec3){0.0f, player->height, 0.0f}, eye);

	int x = (int)SDL_floorf(eye[0]);
	int y = (int)SDL_floorf(eye[1]);
	int z = (int)SDL_floorf(eye[2]);

	float dx = player->front[0];
	float dy = player->front[1];
	float dz = player->front[2];

	int sx = -1;
	if (dx > 0)
		sx = 1;
	int sy = -1;
	if (dy > 0)
		sy = 1;
	int sz = -1;
	if (dz > 0)
		sz = 1;

	float tmx = (eye[0] - SDL_floorf(eye[0])) / -dx;
	if (dx > 0)
		tmx = (SDL_floorf(eye[0]) + 1.0f - eye[0]) / dx;
	float tmy = (eye[1] - SDL_floorf(eye[1])) / -dy;
	if (dy > 0)
		tmy = (SDL_floorf(eye[1]) + 1.0f - eye[1]) / dy;
	float tmz = (eye[2] - SDL_floorf(eye[2])) / -dz;
	if (dz > 0)
		tmz = (SDL_floorf(eye[2]) + 1.0f - eye[2]) / dz;

	float tdx = SDL_fabsf(1.0f / dx);
	float tdy = SDL_fabsf(1.0f / dy);
	float tdz = SDL_fabsf(1.0f / dz);

	float dist = 0.0f;
	int axis = 0;

	while (dist < MC_PLAYER_REACH) {
		int lx = x & (MC_CHUNK_WIDTH - 1);
		int lz = z & (MC_CHUNK_WIDTH - 1);
		if (y < 0 || y >= MC_CHUNK_HEIGHT)
			return;
		mc_block block = chunk->blocks[mc_block_index(lx, y, lz)];
		if (block != MC_AIR_BLOCK) {
			raycast->x = x;
			raycast->y = y;
			raycast->z = z;
			raycast->nx = 0;
			if (axis == 0)
				raycast->nx = -sx;
			raycast->ny = 0;
			if (axis == 1)
				raycast->ny = -sy;
			raycast->nz = 0;
			if (axis == 2)
				raycast->nz = -sz;
			raycast->hit = true;
			return;
		}

		if (tmx < tmy) {
			if (tmx < tmz) {
				dist = tmx;
				tmx += tdx;
				x += sx;
				axis = 0;
			} else {
				dist = tmz;
				tmz += tdz;
				z += sz;
				axis = 2;
			}
		} else {
			if (tmy < tmz) {
				dist = tmy;
				tmy += tdy;
				y += sy;
				axis = 1;
			} else {
				dist = tmz;
				tmz += tdz;
				z += sz;
				axis = 2;
			}
		}
	}
}

void mc_update_player(mc_player* player, mc_player_input* input, mc_chunk* chunk) {
	switch (player->mode) {
		case MC_SURVIVAL_MODE:
			mc_update_survival_player(player, input, chunk);
			break;
		case MC_SPECTATOR_MODE:
			mc_update_spectator_player(player, input);
			break;
		default:
			SDL_Log("Invalid player move.");
			break;
	}
}

void mc_update_survival_player(mc_player* player, mc_player_input* input, mc_chunk* chunk) {
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

	player->front[0] = SDL_cosf(yaw) * SDL_cosf(pitch);
	player->front[1] = SDL_sinf(pitch);
	player->front[2] = SDL_sinf(yaw) * SDL_cosf(pitch);
	glm_vec3_normalize(player->front);

	vec3 globalUp = {0.0f, 1.0f, 0.0f};
	glm_vec3_cross(player->front, globalUp, player->right);
	glm_vec3_normalize(player->right);
	glm_vec3_cross(player->right, player->front, player->up);
	glm_vec3_normalize(player->up);

	vec3 moveFront = {player->front[0], 0.0f, player->front[2]};
	vec3 moveRight = {player->right[0], 0.0f, player->right[2]};
	if (glm_vec3_norm2(moveFront) > 0.0f)
		glm_vec3_normalize(moveFront);
	if (glm_vec3_norm2(moveRight) > 0.0f)
		glm_vec3_normalize(moveRight);

	vec3 direction;
	glm_vec3_zero(direction);
	if (input->forward)
		glm_vec3_add(direction, moveFront, direction);
	if (input->backward)
		glm_vec3_sub(direction, moveFront, direction);
	if (input->left)
		glm_vec3_sub(direction, moveRight, direction);
	if (input->right)
		glm_vec3_add(direction, moveRight, direction);

	if (glm_vec3_norm2(direction) > 0.0f) {
		glm_vec3_normalize(direction);
		float accel = player->grounded ? MC_PLAYER_ACCELERATION : (MC_PLAYER_ACCELERATION * 0.2f);
		if (player->crouched)
			accel *= MC_PLAYER_CROUCH_ACCELERATION;
		player->velocity[0] += direction[0] * accel;
		player->velocity[2] += direction[2] * accel;
	}

	float friction = player->grounded ? MC_PLAYER_FRICTION : MC_PLAYER_AIR_FRICTION;
	player->velocity[0] *= friction;
	player->velocity[2] *= friction;

	float length = SDL_sqrtf(player->velocity[0] * player->velocity[0] + player->velocity[2] * player->velocity[2]);
	if (length > MC_PLAYER_MAX_SPEED) {
		float scale = MC_PLAYER_MAX_SPEED / length;
		player->velocity[0] *= scale;
		player->velocity[2] *= scale;
	}

	player->velocity[1] += MC_PLAYER_GRAVITY;
	if (player->velocity[1] <= MC_PLAYER_TERMINAL_VELOCITY)
		player->velocity[1] = MC_PLAYER_TERMINAL_VELOCITY;

	player->position[0] += player->velocity[0];
	if (mc_collide_player(player, chunk)) {
		player->position[0] -= player->velocity[0];
		player->velocity[0] = 0.0f;
	} else if (player->crouched && player->grounded) {
		player->position[1] -= 1.0f;
		bool grounded = mc_collide_player(player, chunk);
		player->position[1] += 1.0f;
		if (!grounded) {
			player->position[0] -= player->velocity[0];
			player->velocity[0] = 0.0f;
		}
	}

	player->position[2] += player->velocity[2];
	if (mc_collide_player(player, chunk)) {
		player->position[2] -= player->velocity[2];
		player->velocity[2] = 0.0f;
	} else if (player->crouched && player->grounded) {
		player->position[1] -= 1.0f;
		bool grounded = mc_collide_player(player, chunk);
		player->position[1] += 1.0f;
		if (!grounded) {
			player->position[2] -= player->velocity[2];
			player->velocity[2] = 0.0f;
		}
	}

	player->grounded = false;
	player->position[1] += player->velocity[1];
	if (mc_collide_player(player, chunk)) {
		float old_vel_y = player->velocity[1];
		player->position[1] -= old_vel_y;
		player->velocity[1] = 0.0f;
		if (old_vel_y < 0.0f) {
			player->grounded = true;
			player->position[1] = SDL_floorf(player->position[1]);
		}
	}

	if (input->up && player->grounded) {
		player->velocity[1] = MC_PLAYER_JUMP_VELOCITY;
		player->grounded = false;
	}

	if (input->down && player->grounded) {
		player->targetHeight = MC_PLAYER_CROUCH_HEIGHT;
		player->crouched = true;
	} else {
		player->targetHeight = MC_PLAYER_HEIGHT;
		player->crouched = false;
	}

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

	if (input->lmb) {
		mc_player_raycast raycast;
		mc_raycast_player(player, &raycast, chunk);
		if (raycast.hit) {
			int lx = raycast.x & (MC_CHUNK_WIDTH - 1);
			int lz = raycast.z & (MC_CHUNK_HEIGHT - 1);
			chunk->blocks[mc_block_index(lx, raycast.y, lz)] = MC_AIR_BLOCK;
			input->lmb = false;
			chunk->dirty = true;
		}
	}

	if (input->rmb) {
		mc_player_raycast raycast;
		mc_raycast_player(player, &raycast, chunk);
		if (raycast.hit) {
			int x = raycast.x + raycast.nx;
			int y = raycast.y + raycast.ny;
			int z = raycast.z + raycast.nz;
			if (y >= 0 && y <= MC_CHUNK_HEIGHT) {
				int lx = x & (MC_CHUNK_WIDTH - 1);
				int lz = z & (MC_CHUNK_WIDTH - 1);
				mc_block old = chunk->blocks[mc_block_index(lx, y, lz)];
				chunk->blocks[mc_block_index(lx, y, lz)] = player->block;
				if (mc_collide_player(player, chunk)) {
					chunk->blocks[mc_block_index(lx, y, lz)] = old;
				} else {
					input->rmb = false;
					chunk->dirty = true;
				}
			}
		}
	}
}

void mc_update_spectator_player(mc_player* player, mc_player_input* input) {
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

	player->front[0] = SDL_cosf(yaw) * SDL_cosf(pitch);
	player->front[1] = SDL_sinf(pitch);
	player->front[2] = SDL_sinf(yaw) * SDL_cosf(pitch);
	glm_vec3_normalize(player->front);

	vec3 globalUp = {0.0f, 1.0f, 0.0f};

	glm_vec3_cross(player->front, globalUp, player->right);
	glm_vec3_normalize(player->right);

	glm_vec3_cross(player->right, player->front, player->up);
	glm_vec3_normalize(player->up);

	vec3 moveFront = {player->front[0], 0.0f, player->front[2]};
	vec3 moveRight = {player->right[0], 0.0f, player->right[2]};
	vec3 moveUp = {0.0f, player->up[1], 0.0f};
	if (glm_vec3_norm2(moveFront) > 0.0f)
		glm_vec3_normalize(moveFront);
	if (glm_vec3_norm2(moveRight) > 0.0f)
		glm_vec3_normalize(moveRight);
	if (glm_vec3_norm2(moveUp) > 0.0f)
		glm_vec3_normalize(moveUp);

	glm_vec3_scale(player->velocity, MC_PLAYER_FRICTION, player->velocity);

	vec3 direction;
	glm_vec3_zero(direction);
	if (input->forward)
		glm_vec3_add(direction, moveFront, direction);
	if (input->backward)
		glm_vec3_sub(direction, moveFront, direction);
	if (input->left)
		glm_vec3_sub(direction, moveRight, direction);
	if (input->right)
		glm_vec3_add(direction, moveRight, direction);
	if (input->up)
		glm_vec3_add(direction, moveUp, direction);
	if (input->down)
		glm_vec3_sub(direction, moveUp, direction);

	if (glm_vec3_norm2(direction) > 0.0f) {
		glm_vec3_normalize(direction);
		glm_vec3_scale(direction, MC_PLAYER_ACCELERATION, direction);
		glm_vec3_add(player->velocity, direction, player->velocity);
		float length = glm_vec3_norm(player->velocity);
		if (length > MC_PLAYER_MAX_SPEED) {
			float scale = MC_PLAYER_MAX_SPEED / length;
			glm_vec3_scale(player->velocity, scale, player->velocity);
		}
	} else {
		if (SDL_fabsf((player->velocity[0]) < 0.01f))
			player->velocity[0] = 0.0f;
		if (SDL_fabsf(player->velocity[1]) < 0.01f)
			player->velocity[1] = 0.0f;
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
