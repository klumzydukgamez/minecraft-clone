/* Copyright (c) 2026 Klumzy Duk Gamez.
 * See LICENSE.md for full terms. */

#version 330 core

centroid in vec3 vNormal;
centroid in vec2 vTexCoord;

out vec4 oColor;

uniform sampler2D uTexture;

void main(void) {
	vec4 base = texture(uTexture, vTexCoord);
	vec3 direction = normalize(vec3(0.4, 1.0, 0.3));
	float ambient = 0.3;
	float diffuse = max(dot(normalize(vNormal), direction), 0.0);
	vec4 final = base * (diffuse + ambient);
	oColor = vec4(final);
}
