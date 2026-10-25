/* Copyright (c) 2026 Klumzy Duk Gamez.
 * See LICENSE.md for full terms. */

#version 330 core

layout (location = 0) in vec3 aPosition;
layout (location = 1) in vec3 aNormal;

out vec3 vNormal;

uniform mat4 uProjectionView;

void main(void) {
	vNormal = aNormal;
	gl_Position = uProjectionView * vec4(aPosition, 1.0);
}
