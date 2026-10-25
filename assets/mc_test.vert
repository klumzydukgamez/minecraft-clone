/* Copyright (c) 2026 Klumzy Duk Gamez.
 * See LICENSE.md for full terms. */

#version 330 core

uniform mat4 uProjectionView;

void main(void) {
	const vec3 positions[36] = vec3[](
		/* front */
		vec3( 0.0,  0.0,  1.0),
		vec3( 1.0,  0.0,  1.0),
		vec3( 1.0,  1.0,  1.0),
		vec3( 1.0,  1.0,  1.0),
		vec3( 0.0,  1.0,  1.0),
		vec3( 0.0,  0.0,  1.0),
		/* back */
		vec3( 1.0,  0.0,  0.0),
		vec3( 0.0,  0.0,  0.0),
		vec3( 0.0,  1.0,  0.0),
		vec3( 0.0,  1.0,  0.0),
		vec3( 1.0,  1.0,  0.0),
		vec3( 1.0,  0.0,  0.0),
		/* left */
		vec3( 0.0,  0.0,  0.0),
		vec3( 0.0,  0.0,  1.0),
		vec3( 0.0,  1.0,  1.0),
		vec3( 0.0,  1.0,  1.0),
		vec3( 0.0,  1.0,  0.0),
		vec3( 0.0,  0.0,  0.0),
		/* right */
		vec3( 1.0,  0.0,  1.0),
		vec3( 1.0,  0.0,  0.0),
		vec3( 1.0,  1.0,  0.0),
		vec3( 1.0,  1.0,  0.0),
		vec3( 1.0,  1.0,  1.0),
		vec3( 1.0,  0.0,  1.0),
		/* top */
		vec3( 0.0,  1.0,  1.0),
		vec3( 1.0,  1.0,  1.0),
		vec3( 1.0,  1.0,  0.0),
		vec3( 1.0,  1.0,  0.0),
		vec3( 0.0,  1.0,  0.0),
		vec3( 0.0,  1.0,  1.0),
		/* bottom */
		vec3( 0.0,  0.0,  0.0),
		vec3( 1.0,  0.0,  0.0),
		vec3( 1.0,  0.0,  1.0),
		vec3( 1.0,  0.0,  1.0),
		vec3( 0.0,  0.0,  1.0),
		vec3( 0.0,  0.0,  0.0)
	);
	gl_Position = uProjectionView * vec4(positions[gl_VertexID], 1.0);
}
