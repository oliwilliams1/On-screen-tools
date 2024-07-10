#pragma once
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <tesseract/baseapi.h>
#include <opencv2/opencv.hpp>

#include <iostream>
#include <memory>
#include <fstream>
#include <vector>

#include "objectVariants.h"
#include "vec2.h"
#include "screenshotTaker.h"
#include "ocrHandler.h"

class App
{
public:
    static App* instance;

private:

	std::vector<std::unique_ptr<baseObject>> objects;

	float currentTime = 0.0f;
	float previousTime = 0.0f;
	bool debug = false;
	vec2 windowSize = (debug) ? vec2(800, 600) : vec2(1920, 1080);
	float aspectRatio = windowSize.x / windowSize.y;
	vec2 firstCorner;
	vec2 secondCorner;
	vec2 mousePos;
	bool mouseDown;
	vec2 widthHeight = vec2(windowSize.x / windowSize.x, windowSize.y / windowSize.y);

    vec2 toNDC(vec2 value)
    {
        return vec2(value.x * 2 - 1, value.y * 2 - 1);
    }

    static void mousePosCallback(int x, int y)
    {
        // Get mouse coords between 0 to 1
        instance->mousePos = vec2((x / instance->windowSize.x), (y / instance->windowSize.y));

        // To NDC
        instance->mousePos = instance->toNDC(instance->mousePos);

        // Idk why it works but it does
        instance->mousePos.y += 1.0f;
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

    static void renderCB()
    {
        glClear(GL_COLOR_BUFFER_BIT);

        // Calculate the delta time
        instance->currentTime = static_cast<float>(glutGet(GLUT_ELAPSED_TIME)) / 1000.0f; // Elapsed time in seconds
        float deltaTime = instance->currentTime - instance->previousTime;
        instance->previousTime = instance->currentTime;

        if (instance->mouseDown) {
            instance->secondCorner = instance->mousePos - instance->firstCorner;
        }

        for (const auto& object : instance->objects) {
            object->draw();
        }

        glutPostRedisplay();
        glutSwapBuffers();
    }

    static void mouseCB(int button, int state, int x, int y)
    {
        if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
            instance->mouseDown = true;
            instance->firstCorner = instance->mousePos;
            instance->secondCorner = vec2(0);
        }

        if (button == GLUT_LEFT_BUTTON && state == GLUT_UP) {
            instance->mouseDown = false;
        }
    }

    template <typename T>
    void addObject(const T& object)
    {
        static_assert(std::is_base_of_v<baseObject, std::decay_t<T>>, "Object must inherit from basicObject");
        objects.emplace_back(std::make_unique<std::decay_t<T>>(object));
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

        const vec2 fullRectVertices[] = {
            {-1.0f, -1.0f},
            {-1.0f,  1.0f},
            { 1.0f,  1.0f},
            { 1.0f, -1.0f}
        };

        const char* fullRectVertexShaderSource = loadShaderSource("vert.glsl");
        const char* fullRectFragmentShaderSource = loadShaderSource("frag2.glsl");

        advancedObject screenObjects(fullRectVertices, 4, rectIndices, 6, rectUVs, 4, fullRectVertexShaderSource, fullRectFragmentShaderSource, imageData, imageType(1920, 1080));
        addObject(screenObjects);

        fractalRect fractalRectObject(rectVertices, 4, rectIndices, 6, rectUVs, 4, rectVertexShaderSource, rectFragmentShaderSource, &currentTime);
        addObject(fractalRectObject);

        const vec2 smallRectVertices[] = {
            { 0.0f,  0.0f},
            { 0.0f,  0.1f},
            { 0.1f,  0.1f},
            { 0.1f,  0.0f}
        };

        const char* smallRectVertexShaderSource = loadShaderSource("vert2.glsl");
        const char* smallRectFragmentShaderSource = loadShaderSource("frag3.glsl");

        const vec2 drawRectVertices[] = {
            { 0.0f,  0.0f},
            { 0.0f, -1.0f},
            { 1.0f, -1.0f},
            { 1.0f,  0.0f}
        };

        drawRect smallRect(drawRectVertices, 4, rectIndices, 6, rectUVs, 4, smallRectVertexShaderSource, smallRectFragmentShaderSource, &firstCorner, &secondCorner);
        addObject(smallRect);
    }

public:
    App(int *argc, char** argv)
    {
        if (instance == nullptr) {
            instance = this;
        }
        ScreenshotTaker screenshotTaker;
        std::vector<uint8_t> imageData = screenshotTaker.takeScreenshot();

        glutInit(argc, argv);
        if (debug) {
            glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
        }
        else {
            glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_BORDERLESS);
        }

        glutInitWindowSize(windowSize.x, windowSize.y);
        glutInitWindowPosition(0, 0);
        glutCreateWindow("OpenGL Example");

        glutPassiveMotionFunc(mousePosCallback);
        glutMotionFunc(mousePosCallback);

        glewInit();
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        initScene(imageData);
    }

    void run()
    {
        glutDisplayFunc(renderCB);
        glutMouseFunc(mouseCB);
        glutMainLoop();
    }
};

