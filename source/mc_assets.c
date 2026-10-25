/* Copyright (c) 2026 Klumzy Duk Gamez.
 * See LICENSE.md for full terms. */

#include "mc_assets.h"

static const GLchar asset_test_vert[] = {
#embed "mc_test.vert" suffix(, 0)
};
static const GLchar asset_test_frag[] = {
#embed "mc_test.frag" suffix(, 0)
};
const GLchar* const mc_asset_test_vert = asset_test_vert;
const GLchar* const mc_asset_test_frag = asset_test_frag;
