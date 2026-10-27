/* Copyright (c) 2026 Klumzy Duk Gamez.
 * See LICENSE.md for full terms. */

#if !defined MC_PLAYER_H
#define MC_PLAYER_H

#include "mc_third_party.h"
#include "mc_world.h"

typedef enum : uint32_t {
	MC_SURVIVAL_MODE,
	MC_SPECTATOR_MODE
} mc_player_mode;

typedef struct {
	vec3 position;
	vec3 velocity;
	mc_player_mode mode;
	mc_block block;
	bool crouched;
	bool grounded;

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
	bool up;
	bool down;
	bool lmb;
	bool rmb;
} mc_player_input;

typedef struct {
	int x;
	int y;
	int z;
	int nx;
	int ny;
	int nz;
	bool hit;
} mc_player_raycast;

void mc_init_player(mc_player* player);

bool mc_collide_player(mc_player* player, mc_world* world);
void mc_raycast_player(mc_player* player, mc_player_raycast* raycast, mc_world* world);

void mc_update_player(mc_player* player, mc_player_input* input, mc_world* world);
void mc_update_survival_player(mc_player* player, mc_player_input* input, mc_world* world);
void mc_update_spectator_player(mc_player* player, mc_player_input* input);

#endif /* MC_PLAYER_H */
