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
	// Dumb static thing but kinda cool
	static App* instance;

    App(int* argc, char** argv);
	void run();

private:
	// Dynamic object array
	std::vector<std::unique_ptr<baseObject>> objects;

	// Member variables
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

	// Helper functions
	vec2 toNDC(vec2 value);
	const char* loadShaderSource(const char* filename);

	// Stupid callback functions
	static void mousePosCallback(int x, int y);
	static void renderCB();
	static void mouseCB(int button, int state, int x, int y);

	// Easy way to add objects to vector
	template <typename T>
	void addObject(const T& object);

	// Init all objects for rendering
	void initScene(std::vector<uint8_t> imageData);
};

