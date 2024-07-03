#include <GL/glew.h>
#include <GL/freeglut.h>
#include <iostream>
#include <fstream>
#include "basicObject.h"
#include "vector"
#include "vec2.h"
#include "screenshotTaker.h"

std::vector<basicObject> basicObjects;
std::vector<advancedObject> advancedObjects;

float currentTime = 0.0f;
float previousTime = 0.0f;
vec2 mousePos;
bool debug = true;

void mousePosCallback(int x, int y)
{
    mousePos = vec2((x / ((debug) ? 800.0f : 1920.0f)) * 2 - 1, (y / ((debug) ? 600.0f : 1080.0f)) * 2 - 1); // Normalize mouse position
}

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

    // Calculate the delta time
    currentTime = static_cast<float>(glutGet(GLUT_ELAPSED_TIME)) / 1000.0f; // Elapsed time in seconds
    float deltaTime = currentTime - previousTime;
    previousTime = currentTime;

    for (int i = 0; i < advancedObjects.size(); i++) {
        advancedObjects[i].draw();
    }

    for (int i = 0; i < basicObjects.size(); i++) {
        basicObjects[i].draw();
    }

    glutPostRedisplay();
    glutSwapBuffers();
}

void initScene(std::vector<uint8_t> imageData)
{
    const vec2 rectVertices[] = {
        {-0.5f, -0.5f},
        {-0.5f,  0.5f},
        { 0.5f,  0.5f},
        { 0.5f, -0.5f}
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

	const char* rectVertexShaderSource = loadShaderSource("vert.glsl");
	const char* rectFragmentShaderSource = loadShaderSource("frag.glsl");

    basicObject rect(rectVertices, 4, rectIndices, 6, rectUVs, 4, rectVertexShaderSource, rectFragmentShaderSource, &currentTime, &mousePos);
    basicObjects.push_back(rect);

    const vec2 fullRectVertices[] = {
	    {-1.0f, -1.0f},
	    {-1.0f,  1.0f},
	    { 1.0f,  1.0f},
	    { 1.0f, -1.0f}
	};

    const char* fullRectVertexShaderSource = loadShaderSource("vert.glsl");
    const char* fullRectFragmentShaderSource = loadShaderSource("frag2.glsl");

    advancedObject screenObjects(fullRectVertices, 4, rectIndices, 6, rectUVs, 4, fullRectVertexShaderSource, fullRectFragmentShaderSource, &currentTime, &mousePos, imageData, imageType(1920, 1080));
    advancedObjects.push_back(screenObjects);

    const vec2 smallRectVertices[] = {
        { 0.0f,  0.0f},
		{ 0.0f,  0.1f},
		{ 0.1f,  0.1f},
		{ 0.1f,  0.0f}
    };

	const char* smallRectVertexShaderSource = loadShaderSource("vert3.glsl");
	const char* smallRectFragmentShaderSource = loadShaderSource("frag3.glsl");

    basicObject smallRect(smallRectVertices, 4, rectIndices, 6, rectUVs, 4, smallRectVertexShaderSource, smallRectFragmentShaderSource, &currentTime, &mousePos);
	basicObjects.push_back(smallRect);
}

int main(int argc, char** argv)
{
    ScreenshotTaker screenshotTaker;
    std::vector<uint8_t> imageData = screenshotTaker.takeScreenshot();

    glutInit(&argc, argv);
    if (debug) {
		glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    }
    else {
        glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_BORDERLESS);
    }

    glutInitWindowSize((debug) ? 800.0f : 1920.0f, (debug) ? 600.0f : 1080.0f);
    glutInitWindowPosition(0, 0);
    glutCreateWindow("OpenGL Example");

    glutPassiveMotionFunc(mousePosCallback);
    glutMotionFunc(mousePosCallback);

    glewInit();
    initScene(imageData);

    glutDisplayFunc(renderCB);
    glutMainLoop();
    return 0;
}