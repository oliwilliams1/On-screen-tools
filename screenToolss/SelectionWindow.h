#pragma once
#include "vec2.h"
#include "objectVariants.h"
#include "opencv2/core.hpp"
#include <opencv2/highgui.hpp>

class SelectionWindow
{
private:
	vec2* mousePos = nullptr;
	vec2* windowSize = nullptr;
	drawRect* rect = nullptr;
	snoutImage* image = nullptr;

	vec2 cornerOne = vec2(0);
	vec2 cornerTwo = vec2(0);
	bool b_mouseDown = false;

	vec2 toNDC(vec2 value);

public:
	SelectionWindow(drawRect* rect, snoutImage* image, vec2* mousePos, vec2* windowSize);

	SelectionWindow() {};

	void update();

	void mouseDown();

	void mouseUp();
};