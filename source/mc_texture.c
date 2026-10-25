/* Copyright (c) 2026 Klumzy Duk Gamez.
 * See LICENSE.md for full terms. */

#include "mc_texture.h"

bool mc_init_texture(int width, int height, const void* pixels, GLuint* texture) {
	GLuint tex = 0;
	glGenTextures(1, &tex);
	if (!tex) {
		SDL_Log("Failed to generate GL texture.");
		return false;
	}
	glBindTexture(GL_TEXTURE_2D, tex);
	glTexImage2D(
		GL_TEXTURE_2D,
		0,
		GL_RGBA8,
		width, height,
		0,
		GL_RGBA,
		GL_UNSIGNED_BYTE,
		pixels
	);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	*texture = tex;
	return true;
}

bool mc_init_asset_png(const uint8_t* data, size_t size, GLuint* texture) {
	SDL_IOStream* io = SDL_IOFromConstMem(data, size);
	if (!io) {
		SDL_Log("Failed to create SDL IO from asset.");
		return false;
	}
	SDL_Surface* surf = SDL_LoadPNG_IO(io, true);
	if (!surf) {
		SDL_Log("Failed to load SDL surface from IO.");
		return false;
	}
	SDL_Surface* converted = SDL_ConvertSurface(surf, SDL_PIXELFORMAT_RGBA32);
	SDL_DestroySurface(surf);
	if (!converted) {
		SDL_Log("Failed to convert SDL surface.");
		return false;
	}
	if (!mc_init_texture(converted->w, converted->h, converted->pixels, texture)) {
		SDL_Log("Failed to init texture.");
		SDL_DestroySurface(converted);
		return false;
	}
	SDL_DestroySurface(converted);
	return true;
}
