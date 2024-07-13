#include "baseObject.h"

void baseObject::compileShaders(const char* vertexShaderSource, const char* fragmentShaderSource)
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

void baseObject::createBuffers(const vec2* vertices, const GLuint* indices, const vec2* uvCoords)
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

void baseObject::setUniforms() {};

baseObject::baseObject(const vec2* vertices,
	size_t vertexCount,
	const GLuint* indices,
	size_t indexCount,
	const vec2* uvCoords,
	size_t uvCount,
	const char* vertexShaderSource,
	const char* fragmentShaderSource)
{
	compileShaders(vertexShaderSource, fragmentShaderSource);

	this->vertexCount = vertexCount;
	this->indexCount = indexCount;
	this->uvCount = uvCount;

	createBuffers(vertices, indices, uvCoords);

	setUniforms();
}

void baseObject::draw()
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
	glDrawElements(GL_TRIANGLES, (GLsizei)indexCount, GL_UNSIGNED_INT, 0);

	// Disable vertex attributes
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
}