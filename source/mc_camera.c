/* Copyright (c) 2026 Klumzy Duk Gamez.
 * See LICENSE.md for full terms. */

#include "mc_camera.h"
#include "mc_constants.h"

void mc_init_camera(mc_camera* camera) {
	camera->fov = MC_CAMERA_FOV;
	camera->targetFov = camera->fov;

	glm_vec3_zero(camera->position);
	camera->yaw = -90.0f;
	camera->pitch = 0.0f;
	camera->roll = 0.0f;

	glm_vec3_copy((vec3){0.0f, 0.0f, -1.0f}, camera->front);
	glm_vec3_copy((vec3){1.0f, 0.0f, 0.0f}, camera->right);
	glm_vec3_copy((vec3){0.0f, 1.0f, 0.0f}, camera->up);

	glm_mat4_identity(camera->view);
	glm_mat4_identity(camera->projection);
	glm_mat4_identity(camera->viewProjection);

	camera->dirty = true;
}

void mc_calculate_camera(mc_camera* camera) {
	if (SDL_fabsf(camera->fov - camera->targetFov) >= 0.01f) {
		camera->fov = glm_lerp(camera->fov, camera->targetFov, 0.25f);
		camera->dirty = true;
	} else {
		camera->fov = camera->targetFov;
	}

	if (!camera->dirty)
		return;

	float yaw = glm_rad(camera->yaw);
	float pitch = glm_rad(camera->pitch);
	float roll = glm_rad(camera->roll);

	vec3 front;
	front[0] = SDL_cosf(yaw) * SDL_cosf(pitch);
	front[1] = SDL_sinf(pitch);
	front[2] = SDL_sinf(yaw) * SDL_cosf(pitch);
	glm_vec3_normalize_to(front, camera->front);

	vec3 up = {0.0f, 1.0f, 0.0f};
	if (SDL_fabsf(camera->roll) > 0.01f) {
		mat4 rollMat;
		glm_mat4_identity(rollMat);
		glm_rotate(rollMat, roll, camera->front);
		glm_mat4_mulv3(rollMat, up, 1.0f, up);
	}

	glm_vec3_cross(camera->front, up, camera->right);
	glm_vec3_normalize(camera->right);

	glm_vec3_cross(camera->right, camera->front, camera->up);
	glm_vec3_normalize(camera->up);

	vec3 target;
	glm_vec3_add(camera->position, camera->front, target);
	glm_lookat(camera->position, target, camera->up, camera->view);

	glm_perspective(
		glm_rad(camera->fov),
		MC_WINDOW_WIDTH / (float)MC_WINDOW_HEIGHT,
		0.01f,
		100.0f,
		camera->projection
	);

	glm_mat4_mul(camera->projection, camera->view, camera->viewProjection);

	camera->dirty = false;
}

void mc_update_camera(mc_camera* camera, float xrel, float yrel) {
	camera->yaw += xrel;
	camera->pitch += yrel;

	if (camera->pitch > 89.0f)
		camera->pitch = 89.0f;
	if (camera->pitch < -89.0f)
		camera->pitch = -89.0f;

	camera->dirty = true;
}
