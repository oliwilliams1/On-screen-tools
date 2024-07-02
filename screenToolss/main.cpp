#include <GL/glew.h>
#include <GL/freeglut.h>
#include <iostream>
#include <fstream>
#include "basicObject.h"
#include "vector"
#include "vec2.h"
#include "screenshotTaker.h"

std::vector<basicObject> basicObjects;

const char* loadShaderSource(const char* filename) {
    std::ifstream file(filename, std::ios::in | std::ios::binary);
    if (!file.is_open()) {
        return nullptr;
    }

    file.seekg(0, std::ios::end);
    std::streampos file_size = file.tellg();
    file.seekg(0, std::ios::beg);

    char* contents = new char[static_cast<size_t>(file_size) + 1];

    file.read(contents, file_size);
    contents[static_cast<size_t>(file_size)] = '\0';

    file.close();
    return contents;
}

void renderCB()
{
    glClear(GL_COLOR_BUFFER_BIT);

    for (int i = 0; i < basicObjects.size(); i++) {
		basicObjects[i].draw();
	}
    glutPostRedisplay();
    glutSwapBuffers();
}

void initScene()
{
    const vec2 rectVertices[] = {
    {-1.0f, -1.0f},
    {-1.0f,  1.0f},
    { 1.0f,  1.0f},
    { 1.0f, -1.0f}
    };
    const GLuint rectIndices[] = {
        0, 1, 2,
        2, 3, 0
    };

    const vec2 rectUVs[] = {
        {0.0f, 0.0f},
        {0.0f, 1.0f},
        {1.0f, 1.0f},
        {1.0f, 0.0f}
    };

	const char* vertexShaderSource = loadShaderSource("vert.glsl");
	const char* fragmentShaderSource = loadShaderSource("frag.glsl");

    basicObject rect(rectVertices, 4, rectIndices, 6, rectUVs, 4, vertexShaderSource, fragmentShaderSource);
    basicObjects.push_back(rect);
}

int main(int argc, char** argv) 
{
    ScreenshotTaker screenshotTaker;
    std::vector<uint8_t> imageData = screenshotTaker.takeScreenshot();

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);

    glutInitWindowSize(800, 600);
    glutCreateWindow("OpenGL Example");

    glewInit();
    initScene();

    glutDisplayFunc(renderCB);
    glutMainLoop();
    return 0;
}