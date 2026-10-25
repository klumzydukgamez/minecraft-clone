/* Copyright (c) 2026 Klumzy Duk Gamez.
 * See LICENSE.md for full terms. */

#if !defined MC_PLAYER_H
#define MC_PLAYER_H

#include "mc_third_party.h"

typedef struct {
	vec3 position;
	vec3 velocity;

	float fov;
	float targetFov;

	float sensitivity;
	float targetSensitivity;

	float height;
	float targetHeight;

	float yaw;
	float pitch;

	vec3 front;
	vec3 right;
	vec3 up;

	mat4 view;
	mat4 projection;
	mat4 projectionView;
} mc_player;

typedef struct {
	float xrel;
	float yrel;
	bool forward;
	bool backward;
	bool left;
	bool right;
	bool sprint;
} mc_player_input;

void mc_init_player(mc_player* player);
void mc_update_player(mc_player* player, mc_player_input* input);

#endif /* MC_PLAYER_H */
