/* Copyright (c) 2026 Klumzy Duk Gamez.
 * See LICENSE.md for full terms. */

#if !defined MC_CAMERA_H
#define MC_CAMERA_H

#include "mc_third_party.h"

typedef struct {
	float fov;
	float targetFov;

	vec3 position;
	float yaw;
	float pitch;
	float roll;

	vec3 front;
	vec3 right;
	vec3 up;

	mat4 view;
	mat4 projection;
	mat4 projectionView;

	bool dirty;
} mc_camera;

void mc_init_camera(mc_camera* camera);
void mc_calculate_camera(mc_camera* camera);
void mc_update_camera(mc_camera* camera, float xrel, float yrel);

#endif /* MC_CAMERA_H */
