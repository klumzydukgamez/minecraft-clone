/* Copyright (c) 2026 Klumzy Duk Gamez.
 * See LICENSE.md for full terms. */

#version 330 core

in vec3 vNormal;
out vec4 oColor;

void main(void) {
	vec3 base = vec3(1.0, 0.0, 0.0);
	vec3 direction = normalize(vec3(0.4, 1.0, 0.3));
	float ambient = 0.3;
	float diffuse = max(dot(normalize(vNormal), direction), 0.0);
	vec3 final = base * (diffuse + ambient);
	oColor = vec4(final, 1.0);
}
