/* Copyright (c) 2026 Klumzy Duk Gamez.
 * See LICENSE.md for full terms. */

#version 330 core

uniform mat4 uProjectionView;

void main(void) {
	const vec2 positions[3] = vec2[](
		vec2( 0.0,  0.5),
		vec2(-0.5, -0.5),
		vec2( 0.5, -0.5)
	);
	gl_Position = uProjectionView * vec4(positions[gl_VertexID], 0.0, 1.0);
}
