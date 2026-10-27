/* Copyright (c) 2026 Klumzy Duk Gamez.
 * See LICENSE.md for full terms. */

#if !defined MC_CHUNK_H
#define MC_CHUNK_H

#include "mc_constants.h"
#include "mc_block.h"

#define MC_CHUNK_VERTEX(buffer, index, vx, vy, vz, nx, ny, nz, u, v) \
	do {                                                             \
		(buffer)[(index)++] = (GLfloat)(vx);                         \
		(buffer)[(index)++] = (GLfloat)(vy);                         \
		(buffer)[(index)++] = (GLfloat)(vz);                         \
		(buffer)[(index)++] = (GLfloat)(nx);                         \
		(buffer)[(index)++] = (GLfloat)(ny);                         \
		(buffer)[(index)++] = (GLfloat)(nz);                         \
		(buffer)[(index)++] = (GLfloat)(u);                          \
		(buffer)[(index)++] = (GLfloat)(v);                          \
	} while (0)

typedef struct {
	mc_block blocks[MC_CHUNK_BLOCK_COUNT];
	GLuint vertexArray;
	GLuint vertexBuffer;
	int vertexCount;
	int x;
	int z;
	mat4 model;
	bool dirty;
} mc_chunk;

typedef struct {
	GLuint blockTexture;
	GLuint program;
	GLint uProjectionView;
	GLint uTexture;
	GLint uModel;
} mc_chunk_shader;

bool mc_init_chunk_shader(mc_chunk_shader* shader);
void mc_cleanup_chunk_shader(mc_chunk_shader* shader);

int mc_block_index(int x, int y, int z);
void mc_face_uvs(int x, int y, float* u0, float* u1, float* v0, float* v1);

bool mc_init_chunk(mc_chunk* chunk, int x, int z);
bool mc_update_chunk(mc_chunk* chunk, mc_chunk* left, mc_chunk* right, mc_chunk* front, mc_chunk* back);
void mc_draw_chunk(mc_chunk* chunk);
void mc_cleanup_chunk(mc_chunk* chunk);

#endif /* MC_CHUNK_H */
