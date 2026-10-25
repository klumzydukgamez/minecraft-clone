/* Copyright (c) 2026 Klumzy Duk Gamez.
 * See LICENSE.md for full terms. */

#if !defined MC_CHUNK_H
#define MC_CHUNK_H

#include "mc_constants.h"
#include "mc_block.h"

#define MC_CHUNK_VERTEX(buffer, index, vx, vy, vz, nx, ny, nz) \
	(buffer)[(index)++] = (GLfloat)(vx);                       \
	(buffer)[(index)++] = (GLfloat)(vy);                       \
	(buffer)[(index)++] = (GLfloat)(vz);                       \
	(buffer)[(index)++] = (GLfloat)(nx);                       \
	(buffer)[(index)++] = (GLfloat)(ny);                       \
	(buffer)[(index)++] = (GLfloat)(nz);

typedef struct {
	mc_block blocks[MC_CHUNK_BLOCK_COUNT];
	GLuint vertexArray;
	GLuint vertexBuffer;
	int vertexCount;
} mc_chunk;

int mc_block_index(int x, int y, int z);

bool mc_init_chunk(mc_chunk* chunk);
bool mc_update_chunk(mc_chunk* chunk);
void mc_cleanup_chunk(mc_chunk* chunk);

#endif /* MC_CHUNK_H */
