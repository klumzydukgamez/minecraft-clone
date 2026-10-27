/* Copyright (c) 2026 Klumzy Duk Gamez.
 * See LICENSE.md for full terms. */

#if !defined MC_WORLD_H
#define MC_WORLD_H

#include "mc_chunk.h"

typedef struct {
	mc_chunk chunks[MC_WORLD_CHUNKS_WIDTH][MC_WORLD_CHUNKS_WIDTH];
} mc_world;

bool mc_init_world(mc_world* world);
bool mc_update_world(mc_world* world);
void mc_draw_world(mc_world* world, mc_chunk_shader* shader, GLfloat* projectionView);
void mc_cleanup_world(mc_world* world);

mc_chunk* mc_get_world_chunk(mc_world* world, int x, int z);
mc_block mc_get_world_block(mc_world* world, int x, int y, int z);
void mc_set_world_block(mc_world* world, int x, int y, int z, mc_block block);
void mc_dirty_world_chunk(mc_world* world, int x, int z);

#endif /* MC_WORLD_H */
