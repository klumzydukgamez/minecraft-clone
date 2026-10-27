/* Copyright (c) 2026 Klumzy Duk Gamez.
 * See LICENSE.md for full terms. */

#include "mc_world.h"

bool mc_init_world(mc_world* world) {
	bool failed = false;
	for (int i = 0; i < MC_WORLD_CHUNKS_WIDTH; i++) {
		for (int j = 0; j < MC_WORLD_CHUNKS_WIDTH; j++) {
			if (!mc_init_chunk(&world->chunks[i][j], i, j)) {
				SDL_Log("Failed to init chunk.");
				failed = true;
				continue;
			}
		}
	}
	return !failed;
}

bool mc_update_world(mc_world* world) {
	bool failed = false;
	for (int i = 0; i < MC_WORLD_CHUNKS_WIDTH; i++) {
		for (int j = 0; j < MC_WORLD_CHUNKS_WIDTH; j++) {
			mc_chunk* chunk = &world->chunks[i][j];
			if (!chunk->dirty)
				continue;
			mc_chunk* left = mc_get_world_chunk(world, chunk->x - 1, chunk->z);
			mc_chunk* right = mc_get_world_chunk(world, chunk->x + 1, chunk->z);
			mc_chunk* front = mc_get_world_chunk(world, chunk->x, chunk->z + 1);
			mc_chunk* back = mc_get_world_chunk(world, chunk->x, chunk->z - 1);
			if (!mc_update_chunk(&world->chunks[i][j], left, right, front, back)) {
				SDL_Log("Failed to update chunk.");
				failed = true;
				continue;
			}
		}
	}
	return !failed;
}

void mc_draw_world(mc_world* world, mc_chunk_shader* shader, GLfloat* projectionView) {
	glUseProgram(shader->program);
	glUniformMatrix4fv(shader->uProjectionView, 1, GL_FALSE, projectionView);
	glUniform1i(shader->uTexture, 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, shader->blockTexture);
	for (int i = 0; i < MC_WORLD_CHUNKS_WIDTH; i++) {
		for (int j = 0; j < MC_WORLD_CHUNKS_WIDTH; j++) {
			mc_chunk* chunk = &world->chunks[i][j];
			glUniformMatrix4fv(shader->uModel, 1, GL_FALSE, (GLfloat*)chunk->model);
			mc_draw_chunk(chunk);
		}
	}
	glUseProgram(0);
}

void mc_cleanup_world(mc_world* world) {
	for (int i = 0; i < MC_WORLD_CHUNKS_WIDTH; i++) {
		for (int j = 0; j < MC_WORLD_CHUNKS_WIDTH; j++) {
			mc_cleanup_chunk(&world->chunks[i][j]);
		}
	}
}

mc_chunk* mc_get_world_chunk(mc_world* world, int x, int z) {
	for (int i = 0; i < MC_WORLD_CHUNKS_WIDTH; i++) {
		for (int j = 0; j < MC_WORLD_CHUNKS_WIDTH; j++) {
			if (world->chunks[i][j].x == x && world->chunks[i][j].z == z)
				return &world->chunks[i][j];
		}
	}
	return nullptr;
}

mc_block mc_get_world_block(mc_world* world, int x, int y, int z) {
	if (y < 0 || y >= MC_CHUNK_HEIGHT)
		return MC_AIR_BLOCK;
	int cx = ((x - MC_CHUNK_WIDTH + 1) / MC_CHUNK_WIDTH);
	if (x >= 0)
		cx = x / MC_CHUNK_WIDTH;
	int cz = ((z - MC_CHUNK_WIDTH + 1) / MC_CHUNK_WIDTH);
	if (z >= 0)
		cz = z / MC_CHUNK_WIDTH;
	int lx = x & (MC_CHUNK_WIDTH - 1);
	int lz = z & (MC_CHUNK_WIDTH - 1);
	for (int i = 0; i < MC_WORLD_CHUNKS_WIDTH; i++) {
		for (int j = 0; j < MC_WORLD_CHUNKS_WIDTH; j++) {
			mc_chunk* chunk = &world->chunks[i][j];
			if (chunk->x == cx && chunk->z == cz) {
				return chunk->blocks[mc_block_index(lx, y, lz)];
			}
		}
	}
	return MC_AIR_BLOCK;
}

void mc_set_world_block(mc_world* world, int x, int y, int z, mc_block block) {
	if (y < 0 || y >= MC_CHUNK_HEIGHT)
		return;
	int cx = ((x - MC_CHUNK_WIDTH + 1) / MC_CHUNK_WIDTH);
	if (x >= 0)
		cx = x / MC_CHUNK_WIDTH;
	int cz = ((z - MC_CHUNK_WIDTH + 1) / MC_CHUNK_WIDTH);
	if (z >= 0)
		cz = z / MC_CHUNK_WIDTH;
	int lx = x & (MC_CHUNK_WIDTH - 1);
	int lz = z & (MC_CHUNK_WIDTH - 1);
	for (int i = 0; i < MC_WORLD_CHUNKS_WIDTH; i++) {
		for (int j = 0; j < MC_WORLD_CHUNKS_WIDTH; j++) {
			mc_chunk* chunk = &world->chunks[i][j];
			if (chunk->x == cx && chunk->z == cz) {
				chunk->blocks[mc_block_index(lx, y, lz)] = block;
				chunk->dirty = true;
				if (lx == 0)
					mc_dirty_world_chunk(world, cx - 1, cz);
				if (lx == MC_CHUNK_WIDTH - 1)
					mc_dirty_world_chunk(world, cx + 1, cz);
				if (lz == 0)
					mc_dirty_world_chunk(world, cx, cz - 1);
				if (lz == MC_CHUNK_WIDTH - 1)
					mc_dirty_world_chunk(world, cx, cz + 1);
				return;
			}
		}
	}
}

void mc_dirty_world_chunk(mc_world* world, int x, int z) {
	for (int i = 0; i < MC_WORLD_CHUNKS_WIDTH; i++) {
		for (int j = 0; j < MC_WORLD_CHUNKS_WIDTH; j++) {
			if (world->chunks[i][j].x == x && world->chunks[i][j].z == z) {
				world->chunks[i][j].dirty = true;
				return;
			}
		}
	}
}
