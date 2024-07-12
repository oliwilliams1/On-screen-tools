#include "SelectionWindow.h"

SelectionWindow::SelectionWindow(drawRect* rect, vec2* mousePos)
{
    this->rect = rect;
    this->mousePos = mousePos;
    this->cornerOne = rect->cornerOne;
    this->cornerTwo = rect->cornerTwo;
}

void SelectionWindow::update()
{
    if (b_mouseDown)
    {
        rect->cornerOne = cornerOne;
        rect->cornerTwo = *mousePos - cornerOne;
    }
    else
    {
        rect->cornerOne = cornerOne;
        rect->cornerTwo = cornerTwo;
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