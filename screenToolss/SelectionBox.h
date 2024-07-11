#pragma once
#include "vec2.h"
#include "objectVariants.h"

class SelectionBox
{
private:
	drawRect* selectionBox;
	vec2* mousePos;
	bool* mouseDown;
	bool knownMouseDown = false;

	vec2 cornerOnePx;
	vec2 cornerTwoPx;

public:
	SelectionBox(drawRect* selectionBox, vec2* mousePos, bool* mouseDown);

	void update();
};