/* Copyright (c) 2026 Klumzy Duk Gamez.
 * See LICENSE.md for full terms. */

#if !defined MC_SHADER_H
#define MC_SHADER_H

#include "mc_third_party.h"

bool mc_compile_shader(
	const GLchar* vertexSource,
	const GLchar* fragmentSource,
	GLuint* program
);

#endif /* MC_SHADER_H */
