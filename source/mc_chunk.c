/* Copyright (c) 2026 Klumzy Duk Gamez.
 * See LICENSE.md for full terms. */

#include "mc_chunk.h"
#include "mc_assets.h"
#include "mc_texture.h"
#include "mc_shader.h"

bool mc_init_chunk_shader(mc_chunk_shader* shader) {
	if (!mc_init_asset_png(mc_asset_blocks_png, mc_asset_blocks_png_size, &shader->blockTexture)) {
		SDL_Log("Failed to init asset (mc_blocks.png).");
		return false;
	}
	if (!mc_compile_shader(mc_asset_chunk_vert, mc_asset_chunk_frag, &shader->program)) {
		SDL_Log("Failed to compile shader (mc_test.vert, mc_test.frag)");
		return false;
	}
	shader->uProjectionView = glGetUniformLocation(shader->program, "uProjectionView");
	if (shader->uProjectionView == -1) {
		SDL_Log("Failed to get GL uniform location (uProjectionView)");
		return false;
	}
	shader->uTexture = glGetUniformLocation(shader->program, "uTexture");
	if (shader->uTexture == -1) {
		SDL_Log("Failed to get GL uniform location (uTexture).");
		return false;
	}
	shader->uModel = glGetUniformLocation(shader->program, "uModel");
	if (shader->uModel) {
		SDL_Log("Failed to get GL uniform location (uModel).");
		return false;
	}

	return true;
}

void mc_cleanup_chunk_shader(mc_chunk_shader* shader) {
	if (shader->program)
		glDeleteProgram(shader->program);
	if (shader->blockTexture)
		glDeleteTextures(1, &shader->blockTexture);
}

int mc_block_index(int x, int y, int z) {
	return x | (z << 4) | (y << 8);
}

void mc_face_uvs(int x, int y, float* u0, float* u1, float* v0, float* v1) {
	*u0 = x * MC_BLOCK_FACE_U;
	*u1 = *u0 + MC_BLOCK_FACE_U;
	*v0 = y * MC_BLOCK_FACE_V;
	*v1 = *v0 + MC_BLOCK_FACE_V;
}

bool mc_init_chunk(mc_chunk* chunk, int x, int z) {
	chunk->x = x;
	chunk->z = z;

	for (int i = 0; i < MC_CHUNK_BLOCK_COUNT; i++) {
		chunk->blocks[i] = MC_AIR_BLOCK;
	}

	for (int x = 0; x < MC_CHUNK_WIDTH; x++) {
		for (int z = 0; z < MC_CHUNK_WIDTH; z++) {
			for (int y = 0; y < MC_CHUNK_HEIGHT; y++) {
				mc_block block;
				if (y == 15)
					block = MC_GRASS_BLOCK;
				else if (y < 15 & y > 10)
					block = MC_DIRT_BLOCK;
				else if (y <= 10)
					block = MC_STONE_BLOCK;
				else
					block = MC_AIR_BLOCK;
				chunk->blocks[mc_block_index(x, y, z)] = block;
			}
		}
	}

	chunk->vertexCount = 0;

	glGenVertexArrays(1, &chunk->vertexArray);
	if (!chunk->vertexArray) {
		SDL_Log("Failed to generate GL vertex array.");
		return false;
	}
	glGenBuffers(1, &chunk->vertexBuffer);
	if (!chunk->vertexBuffer) {
		SDL_Log("Failed to generate GL buffer.");
		return false;
	}
	chunk->vertexCount = 0;
	chunk->dirty = true;

	return true;
}

bool mc_update_chunk(mc_chunk* chunk, mc_chunk* left, mc_chunk* right, mc_chunk* front, mc_chunk* back) {
	if (!chunk->dirty)
		return true;

	glm_mat4_identity(chunk->model);
	glm_translate(chunk->model, (vec3){chunk->x * MC_CHUNK_WIDTH, 0, chunk->z * MC_CHUNK_WIDTH});

	void* memory = SDL_malloc(MC_CHUNK_BLOCK_COUNT * 6 * 6 * 8 * sizeof(GLfloat));
	if (!memory) {
		SDL_Log("Failed to allocate chunk vertex staging buffer.");
		return false;
	}
	GLfloat* buffer = (GLfloat*)memory;
	int i = 0;

	for (int y = 0; y < MC_CHUNK_HEIGHT; y++) {
		for (int z = 0; z < MC_CHUNK_WIDTH; z++) {
			for (int x = 0; x < MC_CHUNK_WIDTH; x++) {
				int block = chunk->blocks[mc_block_index(x, y, z)];
				/* front */
				float fu0, fu1, fv0, fv1;
				/* back */
				float bu0, bu1, bv0, bv1;
				/* left */
				float lu0, lu1, lv0, lv1;
				/* right */
				float ru0, ru1, rv0, rv1;
				/* top */
				float tu0, tu1, tv0, tv1;
				/* bottom */
				float bmu0, bmu1, bmv0, bmv1;
				switch (block) {
					case MC_AIR_BLOCK:
						continue;
					case MC_DIRT_BLOCK:
						mc_face_uvs(0, 0, &fu0, &fu1, &fv0, &fv1);
						bu0 = lu0 = ru0 = tu0 = bmu0 = fu0;
						bu1 = lu1 = ru1 = tu1 = bmu1 = fu1;
						bv0 = lv0 = rv0 = tv0 = bmv0 = fv0;
						bv1 = lv1 = rv1 = tv1 = bmv1 = fv1;
						break;
					case MC_GRASS_BLOCK:
						mc_face_uvs(1, 0, &fu0, &fu1, &fv0, &fv1);
						bu0 = lu0 = ru0 = fu0;
						bu1 = lu1 = ru1 = fu1;
						bv0 = lv0 = rv0 = fv0;
						bv1 = lv1 = rv1 = fv1;
						mc_face_uvs(2, 0, &tu0, &tu1, &tv0, &tv1);
						mc_face_uvs(0, 0, &bmu0, &bmu1, &bmv0, &bmv1);
						break;
					case MC_STONE_BLOCK:
						mc_face_uvs(3, 0, &fu0, &fu1, &fv0, &fv1);
						bu0 = lu0 = ru0 = tu0 = bmu0 = fu0;
						bu1 = lu1 = ru1 = tu1 = bmu1 = fu1;
						bv0 = lv0 = rv0 = tv0 = bmv0 = fv0;
						bv1 = lv1 = rv1 = tv1 = bmv1 = fv1;
						break;
					default:
						bu0 = lu0 = ru0 = tu0 = bmu0 = fu0 = 0;
						bu1 = lu1 = ru1 = tu1 = bmu1 = fu1 = 0;
						bv0 = lv0 = rv0 = tv0 = bmv0 = fv0 = 0;
						bv1 = lv1 = rv1 = tv1 = bmv1 = fv1 = 0;
						break;
				}
				/* front */
				bool df = false;
				if (z < MC_CHUNK_WIDTH - 1)
					df = (chunk->blocks[mc_block_index(x, y, z + 1)] == MC_AIR_BLOCK);
				else
					df = (front == nullptr || front->blocks[mc_block_index(x, y, 0)] == MC_AIR_BLOCK);
				if (df) {
					MC_CHUNK_VERTEX(buffer, i, x + 0, y + 0, z + 1, 0, 0, 1, fu0, fv1);
					MC_CHUNK_VERTEX(buffer, i, x + 1, y + 0, z + 1, 0, 0, 1, fu1, fv1);
					MC_CHUNK_VERTEX(buffer, i, x + 1, y + 1, z + 1, 0, 0, 1, fu1, fv0);
					MC_CHUNK_VERTEX(buffer, i, x + 1, y + 1, z + 1, 0, 0, 1, fu1, fv0);
					MC_CHUNK_VERTEX(buffer, i, x + 0, y + 1, z + 1, 0, 0, 1, fu0, fv0);
					MC_CHUNK_VERTEX(buffer, i, x + 0, y + 0, z + 1, 0, 0, 1, fu0, fv1);
				}
				/* back */
				bool db = false;
				if (z > 0)
					db = (chunk->blocks[mc_block_index(x, y, z - 1)] == MC_AIR_BLOCK);
				else
					db = (back == nullptr || back->blocks[mc_block_index(x, y, MC_CHUNK_WIDTH - 1)] == MC_AIR_BLOCK);
				if (db) {
					MC_CHUNK_VERTEX(buffer, i, x + 1, y + 0, z + 0, 0, 0, -1, bu0, bv1);
					MC_CHUNK_VERTEX(buffer, i, x + 0, y + 0, z + 0, 0, 0, -1, bu1, bv1);
					MC_CHUNK_VERTEX(buffer, i, x + 0, y + 1, z + 0, 0, 0, -1, bu1, bv0);
					MC_CHUNK_VERTEX(buffer, i, x + 0, y + 1, z + 0, 0, 0, -1, bu1, bv0);
					MC_CHUNK_VERTEX(buffer, i, x + 1, y + 1, z + 0, 0, 0, -1, bu0, bv0);
					MC_CHUNK_VERTEX(buffer, i, x + 1, y + 0, z + 0, 0, 0, -1, bu0, bv1);
				}
				/* left */
				bool dl = false;
				if (x > 0)
					dl = (chunk->blocks[mc_block_index(x - 1, y, z)] == MC_AIR_BLOCK);
				else
					dl = (left == nullptr || left->blocks[mc_block_index(MC_CHUNK_WIDTH - 1, y, z)] == MC_AIR_BLOCK);
				if (dl) {
					MC_CHUNK_VERTEX(buffer, i, x + 0, y + 0, z + 0, -1, 0, 0, lu0, lv1);
					MC_CHUNK_VERTEX(buffer, i, x + 0, y + 0, z + 1, -1, 0, 0, lu1, lv1);
					MC_CHUNK_VERTEX(buffer, i, x + 0, y + 1, z + 1, -1, 0, 0, lu1, lv0);
					MC_CHUNK_VERTEX(buffer, i, x + 0, y + 1, z + 1, -1, 0, 0, lu1, lv0);
					MC_CHUNK_VERTEX(buffer, i, x + 0, y + 1, z + 0, -1, 0, 0, lu0, lv0);
					MC_CHUNK_VERTEX(buffer, i, x + 0, y + 0, z + 0, -1, 0, 0, lu0, lv1);
				}
				/* right */
				bool dr = false;
				if (x < MC_CHUNK_WIDTH - 1)
					dr = (chunk->blocks[mc_block_index(x + 1, y, z)] == MC_AIR_BLOCK);
				else
					dr = (right == nullptr || right->blocks[mc_block_index(0, y, z)] == MC_AIR_BLOCK);
				if (dr) {
					MC_CHUNK_VERTEX(buffer, i, x + 1, y + 0, z + 1, 1, 0, 0, ru0, rv1);
					MC_CHUNK_VERTEX(buffer, i, x + 1, y + 0, z + 0, 1, 0, 0, ru1, rv1);
					MC_CHUNK_VERTEX(buffer, i, x + 1, y + 1, z + 0, 1, 0, 0, ru1, rv0);
					MC_CHUNK_VERTEX(buffer, i, x + 1, y + 1, z + 0, 1, 0, 0, ru1, rv0);
					MC_CHUNK_VERTEX(buffer, i, x + 1, y + 1, z + 1, 1, 0, 0, ru0, rv0);
					MC_CHUNK_VERTEX(buffer, i, x + 1, y + 0, z + 1, 1, 0, 0, ru0, rv1);
				}
				/* top */
				if (y == MC_CHUNK_HEIGHT - 1 || chunk->blocks[mc_block_index(x, y + 1, z)] == MC_AIR_BLOCK) {
					MC_CHUNK_VERTEX(buffer, i, x + 0, y + 1, z + 1, 0, 1, 0, tu0, tv0);
					MC_CHUNK_VERTEX(buffer, i, x + 1, y + 1, z + 1, 0, 1, 0, tu1, tv0);
					MC_CHUNK_VERTEX(buffer, i, x + 1, y + 1, z + 0, 0, 1, 0, tu1, tv1);
					MC_CHUNK_VERTEX(buffer, i, x + 1, y + 1, z + 0, 0, 1, 0, tu1, tv1);
					MC_CHUNK_VERTEX(buffer, i, x + 0, y + 1, z + 0, 0, 1, 0, tu0, tv1);
					MC_CHUNK_VERTEX(buffer, i, x + 0, y + 1, z + 1, 0, 1, 0, tu0, tv0);
				}
				/* bottom */
				if (y == 0 || chunk->blocks[mc_block_index(x, y - 1, z)] == MC_AIR_BLOCK) {
					MC_CHUNK_VERTEX(buffer, i, x + 0, y + 0, z + 0, 0, -1, 0, bmu0, bmv0);
					MC_CHUNK_VERTEX(buffer, i, x + 1, y + 0, z + 0, 0, -1, 0, bmu1, bmv0);
					MC_CHUNK_VERTEX(buffer, i, x + 1, y + 0, z + 1, 0, -1, 0, bmu1, bmv1);
					MC_CHUNK_VERTEX(buffer, i, x + 1, y + 0, z + 1, 0, -1, 0, bmu1, bmv1);
					MC_CHUNK_VERTEX(buffer, i, x + 0, y + 0, z + 1, 0, -1, 0, bmu0, bmv1);
					MC_CHUNK_VERTEX(buffer, i, x + 0, y + 0, z + 0, 0, -1, 0, bmu0, bmv0);
				}
			}
		}
	}
	chunk->vertexCount = i / 8;
	chunk->dirty = false;

	glBindVertexArray(chunk->vertexArray);
	glBindBuffer(GL_ARRAY_BUFFER, chunk->vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, i * sizeof(GLfloat), buffer, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)(6 * sizeof(GLfloat)));
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	SDL_free(buffer);
	return true;
}

void mc_draw_chunk(mc_chunk* chunk) {
	glBindVertexArray(chunk->vertexArray);
	glDrawArrays(GL_TRIANGLES, 0, chunk->vertexCount);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void mc_cleanup_chunk(mc_chunk* chunk) {
	if (chunk->vertexBuffer)
		glDeleteBuffers(1, &chunk->vertexBuffer);
	if (chunk->vertexArray)
		glDeleteVertexArrays(1, &chunk->vertexArray);
}
