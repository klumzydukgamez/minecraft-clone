/* Copyright (c) 2026 Klumzy Duk Gamez.
 * See LICENSE.md for full terms. */

#version 330 core

layout (location = 0) in vec3 aPosition;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;

centroid out vec3 vNormal;
centroid out vec2 vTexCoord;

uniform mat4 uProjectionView;
uniform mat4 uModel;

void main(void) {
	vNormal = aNormal;
	vTexCoord = aTexCoord;
	gl_Position = uProjectionView * uModel * vec4(aPosition, 1.0);
}
