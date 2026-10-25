/* Copyright (c) 2026 Klumzy Duk Gamez.
 * See LICENSE.md for full terms. */

#include "mc_assets.h"

static const GLchar asset_chunk_vert[] = {
#embed "mc_chunk.vert" suffix(, 0)
};
static const GLchar asset_chunk_frag[] = {
#embed "mc_chunk.frag" suffix(, 0)
};
const GLchar* const mc_asset_chunk_vert = asset_chunk_vert;
const GLchar* const mc_asset_chunk_frag = asset_chunk_frag;
