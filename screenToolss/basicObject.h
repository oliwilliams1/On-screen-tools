#pragma once
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <iostream>
#include <vector>

#include "vec2.h"
#include "imageData.h"

class basicObject {
public:
	GLuint vertexShader, fragmentShader, shaderProgram, vbo, ibo, uvbo, mousePosLocation;
	size_t vertexCount, indexCount, uvCount;
	vec2* mousePos;

	void compileShaders(const char* vertexShaderSource, const char* fragmentShaderSource)
	{
		// Create and compile vertex shader
		vertexShader = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
		glCompileShader(vertexShader);

		// Check for vertex shader compilation errors
		int success;
		char infoLog[512];
		glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
		if (!success) {
			glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
			std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
		}

		// Create and compile fragment shader
		fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
		glCompileShader(fragmentShader);

		// Check for fragment shader compilation errors
		glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
		if (!success) {
			glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
			std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
		}
	}

	void createBuffers(const vec2* vertices, const GLuint* indices, const vec2* uvCoords)
	{
		// Create shader program
		shaderProgram = glCreateProgram();
		glAttachShader(shaderProgram, vertexShader);
		glAttachShader(shaderProgram, fragmentShader);
		glLinkProgram(shaderProgram);

		// Clean up
		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);

		// Create VBO
		glGenBuffers(1, &vbo);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vec2) * vertexCount, vertices, GL_STATIC_DRAW);

		// Create IBO
		glGenBuffers(1, &ibo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * indexCount, indices, GL_STATIC_DRAW);

		// Create UV buffer
		glGenBuffers(1, &uvbo);
		glBindBuffer(GL_ARRAY_BUFFER, uvbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vec2) * uvCount, uvCoords, GL_STATIC_DRAW);
	}

	void setUniforms()
	{
		mousePosLocation = glGetUniformLocation(shaderProgram, "mousePos");
	}
	basicObject(
		const vec2* vertices, 
		size_t vertexCount, 
		const GLuint* indices, 
		size_t indexCount,
		const vec2* uvCoords,
		size_t uvCount,
		const char* vertexShaderSource, 
		const char* fragmentShaderSource,
		vec2* mousePos)
	{
		compileShaders(vertexShaderSource, fragmentShaderSource);

		this->vertexCount = vertexCount;
		this->indexCount = indexCount;
		this->uvCount = uvCount;
		this->mousePos = mousePos;

		createBuffers(vertices, indices, uvCoords);	
		
		setUniforms();
	}

	virtual void updateUniforms() = 0;

	void draw() 
	{
		glUseProgram(shaderProgram);

		// Bind VBO and IBO
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);

		// Enable the vertex attribute
		glEnableVertexAttribArray(0); // position
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(vec2), 0);

		// Bind UV buffer and enable its attribute
		glBindBuffer(GL_ARRAY_BUFFER, uvbo);
		glEnableVertexAttribArray(1); // uv
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(vec2), (void*)sizeof(vec2));

		updateUniforms();

		// Draw geometry
		glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);

		// Disable vertex attributes
		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);
	}
};

class advancedObject : public basicObject
{
private:
	GLuint textureLocation;
	GLuint textureID;

	void createTexture(std::vector<uint8_t> imageData, imageType imageType)
	{
		// Generate and bind the texture object
		glGenTextures(1, &textureID);
		glBindTexture(GL_TEXTURE_2D, textureID);

		// Upload the image data to the texture
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, imageType.width, imageType.height, 0, GL_RGB, GL_UNSIGNED_BYTE, imageData.data());

		// Set the texture parameters
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		// Get the location of the texture uniform variable in the shader program
		textureLocation = glGetUniformLocation(shaderProgram, "texture");
	}

public:
	advancedObject(
		const vec2* vertices,
		size_t vertexCount,
		const GLuint* indices,
		size_t indexCount,
		const vec2* uvCoords,
		size_t uvCount,
		const char* vertexShaderSource,
		const char* fragmentShaderSource,
		vec2* mousePos,
		std::vector<uint8_t> imageData,
		imageType imageType
	) : basicObject(vertices, vertexCount, indices, indexCount, uvCoords, uvCount, vertexShaderSource, fragmentShaderSource, mousePos)
	{
		createTexture(imageData, imageType);
	}

	void updateUniforms()
	{
		// Update uniform variables
		glUniform2f(mousePosLocation, mousePos->x, mousePos->y);
		glBindTexture(GL_TEXTURE_2D, textureID);
		glUniform1i(textureLocation, 0); // Bind the texture to texture unit 0
	}
};

class fractalRect : public basicObject
{
private:
	GLuint timeLocation;
	float* time;

	void setUniforms()
	{
		// Bind the time uniform variable
		timeLocation = glGetUniformLocation(shaderProgram, "time");
	}

	void basicObject::updateUniforms() 
	{
		// Update uniform variables
		glUniform1f(timeLocation, *time);
	}

public:

	fractalRect(
		const vec2* vertices,
		size_t vertexCount,
		const GLuint* indices,
		size_t indexCount,
		const vec2* uvCoords,
		size_t uvCount,
		const char* vertexShaderSource,
		const char* fragmentShaderSource,
		float* time,
		vec2* mousePos
	) : basicObject(vertices, vertexCount, indices, indexCount, uvCoords, uvCount, vertexShaderSource, fragmentShaderSource, mousePos)
	{
		this->time = time;
		setUniforms();
	}
};

class drawRect : public basicObject
{
private:
	void basicObject::updateUniforms() {};
public:
	drawRect(
		const vec2* vertices,
		size_t vertexCount,
		const GLuint* indices,
		size_t indexCount,
		const vec2* uvCoords,
		size_t uvCount,
		const char* vertexShaderSource,
		const char* fragmentShaderSource
	) : basicObject(vertices, vertexCount, indices, indexCount, uvCoords, uvCount, vertexShaderSource, fragmentShaderSource, nullptr) {};
};