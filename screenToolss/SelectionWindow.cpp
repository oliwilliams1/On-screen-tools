#include "SelectionWindow.h"

SelectionWindow::SelectionWindow(drawRect* rect, vec2* mousePos, vec2* windowSize)
{
    this->rect = rect;
    this->mousePos = mousePos;
    this->windowSize = windowSize;
    this->cornerOne = rect->cornerOne;
    this->cornerTwo = rect->cornerTwo;
}

// [0, 1] (px) -> [-1, 1]
vec2 SelectionWindow::toNDC(vec2 value)
{
    value = value / *windowSize; // Normalize to [0, 1]
    value *= 2.0f;               // Scale to  [0, 2]
    value -= 1.0f;               // Translate to [-1, 1]
    return value;
}

void SelectionWindow::update()
{
    if (b_mouseDown)
    {
        rect->cornerOne = toNDC(cornerOne);
        rect->cornerTwo = toNDC(*mousePos) - toNDC(cornerOne);
    }
}

void SelectionWindow::mouseDown()
{
    b_mouseDown = true;
    cornerOne = *mousePos;
    cornerTwo = vec2(0);
}

void SelectionWindow::mouseUp()
{
    b_mouseDown = false;
    cornerOne = rect->cornerOne;
    cornerTwo = rect->cornerTwo;
}