/* Copyright (c) 2026 Klumzy Duk Gamez.
 * See LICENSE.md for full terms. */

#include "mc_shader.h"

bool mc_compile_shader(
	const GLchar* vertexSource,
	const GLchar* fragmentSource,
	GLuint* program
) {
	GLuint vertex = glCreateShader(GL_VERTEX_SHADER);
	if (!vertex) {
		SDL_Log("Failed to create GL vertex shader.");
		return false;
	}
	glShaderSource(vertex, 1, &vertexSource, nullptr);
	glCompileShader(vertex);
	int vertexResult = 0;
	glGetShaderiv(vertex, GL_COMPILE_STATUS, &vertexResult);
	if (!vertexResult) {
		SDL_Log("Failed to compile GL vertex shader.");
		GLchar log[256] = {};
		glGetShaderInfoLog(vertex, sizeof(log), nullptr, log);
		SDL_Log("%s", log);
		glDeleteShader(vertex);
		return false;
	}

	GLuint fragment = glCreateShader(GL_FRAGMENT_SHADER);
	if (!fragment) {
		SDL_Log("Failed to create GL fragment shader.");
		return false;
	}
	glShaderSource(fragment, 1, &fragmentSource, nullptr);
	glCompileShader(fragment);
	int fragmentResult = 0;
	glGetShaderiv(fragment, GL_COMPILE_STATUS, &fragmentResult);
	if (!fragmentResult) {
		SDL_Log("Failed to compile GL fragment shader.");
		GLchar log[256] = {};
		glGetShaderInfoLog(fragment, sizeof(log), nullptr, log);
		SDL_Log("%s", log);
		glDeleteShader(vertex);
		glDeleteShader(fragment);
		return false;
	}

	GLuint prog = glCreateProgram();
	if (!prog) {
		SDL_Log("Failed to create GL program.");
		glDeleteShader(vertex);
		glDeleteShader(fragment);
		return false;
	}
	glAttachShader(prog, vertex);
	glAttachShader(prog, fragment);
	glLinkProgram(prog);
	glDetachShader(prog, vertex);
	glDetachShader(prog, fragment);
	glDeleteShader(vertex);
	glDeleteShader(fragment);
	int programResult = 0;
	glGetProgramiv(prog, GL_LINK_STATUS, &programResult);
	if (!programResult) {
		SDL_Log("Failed to link GL program.");
		GLchar log[256] = {};
		glGetShaderInfoLog(prog, sizeof(log), nullptr, log);
		SDL_Log("%s", log);
		glDeleteProgram(prog);
		return false;
	}

	*program = prog;
	return true;
}
