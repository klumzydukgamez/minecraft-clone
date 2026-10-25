/* Copyright (c) 2026 Klumzy Duk Gamez.
 * See LICENSE.md for full terms. */

#if !defined MC_CHUNK_H
#define MC_CHUNK_H

#include "mc_constants.h"
#include "mc_block.h"

typedef struct {
	mc_block blocks[MC_CHUNK_HEIGHT][MC_CHUNK_WIDTH][MC_CHUNK_WIDTH];
} mc_chunk;

#endif /* MC_CHUNK_H */
