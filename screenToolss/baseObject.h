#pragma once
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <iostream>
#include <vector>
#include "vec2.h"
#include "imageData.h"

class baseObject {
public:
	GLuint vertexShader, fragmentShader, shaderProgram, vbo, ibo, uvbo, mousePosLocation;
	size_t vertexCount, indexCount, uvCount;
	vec2* mousePos;

	void compileShaders(const char* vertexShaderSource, const char* fragmentShaderSource);

	void createBuffers(const vec2* vertices, const GLuint* indices, const vec2* uvCoords);

	void setUniforms();

	baseObject(
		const vec2* vertices,
		size_t vertexCount,
		const GLuint* indices,
		size_t indexCount,
		const vec2* uvCoords,
		size_t uvCount,
		const char* vertexShaderSource,
		const char* fragmentShaderSource,
		vec2* mousePos);

	virtual void updateUniforms() = 0;

	void draw();
};