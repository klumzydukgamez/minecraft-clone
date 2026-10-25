/* Copyright (c) 2026 Klumzy Duk Gamez.
 * See LICENSE.md for full terms. */

#if !defined MC_TEXTURE_H
#define MC_TEXTURE_H

#include "mc_third_party.h"

bool mc_init_texture(int width, int height, const void* pixels, GLuint* texture);

bool mc_init_asset_png(const uint8_t* data, size_t size, GLuint* texture);

#endif /* MC_TEXTURE_H */
