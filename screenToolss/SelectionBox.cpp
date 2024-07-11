#include "SelectionBox.h"

SelectionBox::SelectionBox(drawRect* selectionBox, vec2* mousePos, bool* mouseDown)
{
	this->selectionBox = selectionBox;
	this->mousePos = mousePos;
	this->mouseDown = mouseDown;

	this->cornerOnePx = vec2(0);
	this->cornerTwoPx = vec2(0);

	this->selectionBox->cornerOne = &this->cornerOnePx;
	this->selectionBox->cornerTwo = &this->cornerTwoPx;
}

void SelectionBox::update()
{
	std::cout << (selectionBox->cornerOne == &this->cornerOnePx);
	if (*mouseDown && !knownMouseDown) {
		std::cout << "Mouse down" << std::endl;
		knownMouseDown = true;
		cornerOnePx = *mousePos;
	}

	if (*mouseDown) {
		cornerTwoPx = *mousePos - cornerOnePx;
	}

	if (!*mouseDown) {
		knownMouseDown = false;
	}
}