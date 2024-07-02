#pragma once
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <vector>
#include "vec3.h"
#include "vec2.h"
#include "imageData.h"

GLuint createTexture(std::vector<uint8_t> imageData, imageType imageType)
{
    GLuint textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);

    // Upload the image data to the texture
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, imageType.width, imageType.height, 0, GL_RGB, GL_UNSIGNED_BYTE, imageData.data());

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    return textureID;
}


// Vertex Shader
const char* vertexShaderSource = "#version 460 core\n"
"layout (location = 0) in vec3 position;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(position, 1.0);\n"
"}\0";

// Fragment Shader
const char* fragmentShaderSource = "#version 460 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(0, 1, 0, 1);\n"
"}\0";

void AddShader(GLuint ShaderProgram, const char* pShaderText, GLenum ShaderType)
{
    GLuint ShaderObj = glCreateShader(ShaderType);

    if (ShaderObj == 0) {
        fprintf(stderr, "Error creating shader type %d\n", ShaderType);
        exit(1);
    }

    const GLchar* p[1];
    p[0] = pShaderText;

    GLint Lengths[1];
    Lengths[0] = (GLint)strlen(pShaderText);

    glShaderSource(ShaderObj, 1, p, Lengths);

    glCompileShader(ShaderObj);

    GLint success;
    glGetShaderiv(ShaderObj, GL_COMPILE_STATUS, &success);

    if (!success) {
        GLchar InfoLog[1024];
        glGetShaderInfoLog(ShaderObj, 1024, NULL, InfoLog);
        fprintf(stderr, "Error compiling shader type %d: '%s'\n", ShaderType, InfoLog);
        exit(1);
    }

    glAttachShader(ShaderProgram, ShaderObj);
}

class Object
{
private:
    GLuint m_vbo;
    GLuint m_ibo;
    GLuint m_shaderProgram;
    std::vector<vec3> m_vertices;
    std::vector<vec3> m_indices;

public:
    Object() {};
    Object(std::vector<vec3> vertices, std::vector<vec3> indices, std::vector<uint8_t> imageData, imageType imageType)
        : m_vertices(vertices), m_indices(indices)
    {
        glGenBuffers(1, &m_vbo);
        glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vec3) * m_vertices.size(), m_vertices.data(), GL_STATIC_DRAW);

        glGenBuffers(1, &m_ibo);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ibo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(vec3) * m_indices.size(), m_indices.data(), GL_STATIC_DRAW);

        // Load and compile the shaders
        m_shaderProgram = glCreateProgram();

        if (m_shaderProgram == 0) {
            fprintf(stderr, "Error creating shader program\n");
            exit(1);
        }

        AddShader(m_shaderProgram, vertexShaderSource, GL_VERTEX_SHADER);
        AddShader(m_shaderProgram, fragmentShaderSource, GL_FRAGMENT_SHADER);

        GLint Success = 0;
        GLchar ErrorLog[1024] = { 0 };

        glLinkProgram(m_shaderProgram);

        glGetProgramiv(m_shaderProgram, GL_LINK_STATUS, &Success);

        glValidateProgram(m_shaderProgram);
        glGetProgramiv(m_shaderProgram, GL_VALIDATE_STATUS, &Success);
        if (!Success) {
            glGetProgramInfoLog(m_shaderProgram, sizeof(ErrorLog), NULL, ErrorLog);
            fprintf(stderr, "Invalid shader program: '%s'\n", ErrorLog);
            exit(1);
        }
    }

    void render()
    {
        glUseProgram(m_shaderProgram);

        glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ibo);

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

        glDrawElements(GL_TRIANGLES, m_indices.size(), GL_UNSIGNED_INT, 0);

        glDisableVertexAttribArray(0);
    }
};