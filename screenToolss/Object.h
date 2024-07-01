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
const char* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 position;\n"
"layout (location = 1) in vec2 texCoord;\n"
"out vec2 TexCoord;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(position, 1.0);\n"
"   TexCoord = texCoord;\n"
"}\0";

// Fragment Shader
const char* fragmentShaderSource = "#version 330 core\n"
"in vec2 TexCoord;\n"
"out vec4 FragColor;\n"
"uniform sampler2D textureSampler;\n"
"void main()\n"
"{\n"
"   FragColor = texture(textureSampler, TexCoord);\n"
"}\0";

// Helper function to load and compile shaders
GLuint loadAndCompileShaders(const char* vertexShaderSource, const char* fragmentShaderSource)
{
    // Vertex Shader
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    // Fragment Shader
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    // Shader Program
    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    // Clean up
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return shaderProgram;
}

class Object
{
private:
    GLuint m_vbo;
    GLuint m_ibo;
    GLuint m_textureID;
    GLuint m_vao;
    GLuint m_shaderProgram;
    GLint m_positionAttrib;
    GLint m_texCoordAttrib;
    GLint m_textureSamplerUniform;
    vec3 m_position;
    std::vector<vec3> m_vertices;
    std::vector<vec3> m_indices;
    std::vector<vec2> m_texCoords;

public:
    Object(std::vector<vec3> vertices, std::vector<vec3> indices, std::vector<vec2> texCoords, std::vector<uint8_t> imageData, imageType imageType)
        : m_vertices(vertices), m_indices(indices), m_texCoords(texCoords)
    {
        // Create and bind the VBO
        glGenBuffers(1, &m_vbo);
        glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
        glBufferData(GL_ARRAY_BUFFER, (m_vertices.size() + m_texCoords.size()) * sizeof(vec3), nullptr, GL_STATIC_DRAW);
        glBufferSubData(GL_ARRAY_BUFFER, 0, m_vertices.size() * sizeof(vec3), m_vertices.data());
        glBufferSubData(GL_ARRAY_BUFFER, m_vertices.size() * sizeof(vec3), m_texCoords.size() * sizeof(vec2), m_texCoords.data());

        // Create and bind the IBO
        glGenBuffers(1, &m_ibo);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ibo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indices.size() * sizeof(vec3), m_indices.data(), GL_STATIC_DRAW);

        // Create the texture
        m_textureID = createTexture(imageData, imageType);

        // Create the VAO
        glGenVertexArrays(1, &m_vao);
        glBindVertexArray(m_vao);
        glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ibo);

        // Load and compile the shaders
        m_shaderProgram = loadAndCompileShaders("shader.vert", "shader.frag");

        // Get the attribute and uniform locations
        m_positionAttrib = glGetAttribLocation(m_shaderProgram, "position");
        m_texCoordAttrib = glGetAttribLocation(m_shaderProgram, "texCoord");
        m_textureSamplerUniform = glGetUniformLocation(m_shaderProgram, "textureSampler");
    }

    void render()
    {
        // Bind the texture
        glBindTexture(GL_TEXTURE_2D, m_textureID);

        // Bind the VAO
        glBindVertexArray(m_vao);

        // Use the shader program
        glUseProgram(m_shaderProgram);

        // Set the texture uniform
        glUniform1i(m_textureSamplerUniform, 0);

        // Enable vertex attributes
        glEnableVertexAttribArray(m_positionAttrib);
        glEnableVertexAttribArray(m_texCoordAttrib);

        // Draw the object
        glDrawElements(GL_TRIANGLES, m_indices.size(), GL_UNSIGNED_INT, 0);

        // Disable vertex attributes
        glDisableVertexAttribArray(m_positionAttrib);
        glDisableVertexAttribArray(m_texCoordAttrib);

        // Unbind the VAO and shader program
        glBindVertexArray(0);
        glUseProgram(0);
    }
};