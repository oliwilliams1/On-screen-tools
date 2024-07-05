#pragma once
#include "baseObject.h"

class advancedObject : public baseObject
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
	) : baseObject(vertices, vertexCount, indices, indexCount, uvCoords, uvCount, vertexShaderSource, fragmentShaderSource, mousePos)
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

class fractalRect : public baseObject
{
private:
	GLuint timeLocation;
	float* time;

	void setUniforms()
	{
		// Bind the time uniform variable
		timeLocation = glGetUniformLocation(shaderProgram, "time");
	}

	void baseObject::updateUniforms()
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
	) : baseObject(vertices, vertexCount, indices, indexCount, uvCoords, uvCount, vertexShaderSource, fragmentShaderSource, mousePos)
	{
		this->time = time;
		setUniforms();
	}
};

class drawRect : public baseObject
{
private:
	void baseObject::updateUniforms() {};
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
	) : baseObject(vertices, vertexCount, indices, indexCount, uvCoords, uvCount, vertexShaderSource, fragmentShaderSource, nullptr) {};
};