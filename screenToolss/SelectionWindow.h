#pragma once
#include "vec2.h"
#include "objectVariants.h"

class SelectionWindow
{
private:
	vec2* mousePos;
	drawRect* rect;

	vec2 cornerOne = vec2(0);
	vec2 cornerTwo = vec2(0);
	bool b_mouseDown = false;

public:
	SelectionWindow(drawRect* rect, vec2* mousePos);

	SelectionWindow() {};

	void update();

	void mouseDown();

	void mouseUp();
};