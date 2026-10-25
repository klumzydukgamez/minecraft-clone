/* Copyright (c) 2026 Klumzy Duk Gamez.
 * See LICENSE.md for full terms. */

#include "mc_chunk.h"

int mc_block_index(int x, int y, int z) {
	return x | (z << 4) | (y << 8);
}

bool mc_init_chunk(mc_chunk* chunk) {
	for (int i = 0; i < MC_CHUNK_BLOCK_COUNT; i++) {
		chunk->blocks[i] = MC_AIR_BLOCK;
	}

	for (int x = 0; x < MC_CHUNK_WIDTH; x++) {
		for (int z = 0; z < MC_CHUNK_WIDTH; z++) {
			for (int y = 0; y < MC_CHUNK_HEIGHT; y++) {
				chunk->blocks[mc_block_index(x, y, z)] = MC_DIRT_BLOCK;
			}
		}
	}

	chunk->blocks[mc_block_index(0, 0, 0)] = MC_AIR_BLOCK;

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

	return true;
}

bool mc_update_chunk(mc_chunk* chunk) {
	void* memory = SDL_malloc(MC_CHUNK_BLOCK_COUNT * 6 * 6 * 6 * sizeof(GLfloat));
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
				if (block == MC_AIR_BLOCK)
					continue;
				/* front */
				if (z == MC_CHUNK_WIDTH - 1 || chunk->blocks[mc_block_index(x, y, z + 1)] == MC_AIR_BLOCK) {
					MC_CHUNK_VERTEX(buffer, i, x + 0, y + 0, z + 1, 0, 0, 1);
					MC_CHUNK_VERTEX(buffer, i, x + 1, y + 0, z + 1, 0, 0, 1);
					MC_CHUNK_VERTEX(buffer, i, x + 1, y + 1, z + 1, 0, 0, 1);
					MC_CHUNK_VERTEX(buffer, i, x + 1, y + 1, z + 1, 0, 0, 1);
					MC_CHUNK_VERTEX(buffer, i, x + 0, y + 1, z + 1, 0, 0, 1);
					MC_CHUNK_VERTEX(buffer, i, x + 0, y + 0, z + 1, 0, 0, 1);
				}
				/* back */
				if (z == 0 || chunk->blocks[mc_block_index(x, y, z - 1)] == MC_AIR_BLOCK) {
					MC_CHUNK_VERTEX(buffer, i, x + 1, y + 0, z + 0, 0, 0, -1);
					MC_CHUNK_VERTEX(buffer, i, x + 0, y + 0, z + 0, 0, 0, -1);
					MC_CHUNK_VERTEX(buffer, i, x + 0, y + 1, z + 0, 0, 0, -1);
					MC_CHUNK_VERTEX(buffer, i, x + 0, y + 1, z + 0, 0, 0, -1);
					MC_CHUNK_VERTEX(buffer, i, x + 1, y + 1, z + 0, 0, 0, -1);
					MC_CHUNK_VERTEX(buffer, i, x + 1, y + 0, z + 0, 0, 0, -1);
				}
				/* left */
				if (x == 0 || chunk->blocks[mc_block_index(x - 1, y, z)] == MC_AIR_BLOCK) {
					MC_CHUNK_VERTEX(buffer, i, x + 0, y + 0, z + 0, -1, 0, 0);
					MC_CHUNK_VERTEX(buffer, i, x + 0, y + 0, z + 1, -1, 0, 0);
					MC_CHUNK_VERTEX(buffer, i, x + 0, y + 1, z + 1, -1, 0, 0);
					MC_CHUNK_VERTEX(buffer, i, x + 0, y + 1, z + 1, -1, 0, 0);
					MC_CHUNK_VERTEX(buffer, i, x + 0, y + 1, z + 0, -1, 0, 0);
					MC_CHUNK_VERTEX(buffer, i, x + 0, y + 0, z + 0, -1, 0, 0);
				}
				/* right */
				if (x == MC_CHUNK_WIDTH - 1 || chunk->blocks[mc_block_index(x + 1, y, z)] == MC_AIR_BLOCK) {
					MC_CHUNK_VERTEX(buffer, i, x + 1, y + 0, z + 1, 1, 0, 0);
					MC_CHUNK_VERTEX(buffer, i, x + 1, y + 0, z + 0, 1, 0, 0);
					MC_CHUNK_VERTEX(buffer, i, x + 1, y + 1, z + 0, 1, 0, 0);
					MC_CHUNK_VERTEX(buffer, i, x + 1, y + 1, z + 0, 1, 0, 0);
					MC_CHUNK_VERTEX(buffer, i, x + 1, y + 1, z + 1, 1, 0, 0);
					MC_CHUNK_VERTEX(buffer, i, x + 1, y + 0, z + 1, 1, 0, 0);
				}
				/* top */
				if (y == MC_CHUNK_HEIGHT - 1 || chunk->blocks[mc_block_index(x, y + 1, z)] == MC_AIR_BLOCK) {
					MC_CHUNK_VERTEX(buffer, i, x + 0, y + 1, z + 1, 0, 1, 0);
					MC_CHUNK_VERTEX(buffer, i, x + 1, y + 1, z + 1, 0, 1, 0);
					MC_CHUNK_VERTEX(buffer, i, x + 1, y + 1, z + 0, 0, 1, 0);
					MC_CHUNK_VERTEX(buffer, i, x + 1, y + 1, z + 0, 0, 1, 0);
					MC_CHUNK_VERTEX(buffer, i, x + 0, y + 1, z + 0, 0, 1, 0);
					MC_CHUNK_VERTEX(buffer, i, x + 0, y + 1, z + 1, 0, 1, 0);
				}
				/* bottom */
				if (y == 0 || chunk->blocks[mc_block_index(x, y - 1, z)] == MC_AIR_BLOCK) {
					MC_CHUNK_VERTEX(buffer, i, x + 0, y + 0, z + 0, 0, -1, 0);
					MC_CHUNK_VERTEX(buffer, i, x + 1, y + 0, z + 0, 0, -1, 0);
					MC_CHUNK_VERTEX(buffer, i, x + 1, y + 0, z + 1, 0, -1, 0);
					MC_CHUNK_VERTEX(buffer, i, x + 1, y + 0, z + 1, 0, -1, 0);
					MC_CHUNK_VERTEX(buffer, i, x + 0, y + 0, z + 1, 0, -1, 0);
					MC_CHUNK_VERTEX(buffer, i, x + 0, y + 0, z + 0, 0, -1, 0);
				}
			}
		}
	}
	chunk->vertexCount = i / 6;

	glBindVertexArray(chunk->vertexArray);
	glBindBuffer(GL_ARRAY_BUFFER, chunk->vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, i * sizeof(GLfloat), buffer, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	SDL_free(buffer);
	return true;
}

void mc_cleanup_chunk(mc_chunk* chunk) {
	if (chunk->vertexBuffer)
		glDeleteBuffers(1, &chunk->vertexBuffer);
	if (chunk->vertexArray)
		glDeleteVertexArrays(1, &chunk->vertexArray);
}
