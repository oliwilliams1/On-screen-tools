#include "SelectionWindow.h"

SelectionWindow::SelectionWindow(drawRect* rect, snoutImage* image, vec2* mousePos, vec2* windowSize)
{
    this->rect = rect;
    this->mousePos = mousePos;
    this->image = image;
    this->windowSize = windowSize;
    this->cornerOne = rect->cornerOne;
    this->cornerTwo = rect->cornerTwo;
}

float min(float x, float y)
{
	return x < y ? x : y;
}

float max(float x, float y)
{
	return x > y ? x : y;
}

snoutImage cropImage(vec2 _cornerOne, vec2 _cornerTwo, snoutImage* image)
{
    _cornerOne.print();
    _cornerTwo.print();
    vec2 cornerOne = vec2(min(_cornerOne.x, _cornerTwo.x), min(_cornerOne.y, _cornerTwo.y));
    vec2 cornerTwo = vec2(max(_cornerOne.x, _cornerTwo.x), max(_cornerOne.y, _cornerTwo.y));

    int width = (int)cornerTwo.x - (int)cornerOne.x;
    int height = (int)cornerTwo.y - (int)cornerOne.y;
    int numChannels = image->channels;

    // Check if the cropped image dimensions are valid
    if (width <= 0 || height <= 0)
    {
        std::cerr << "Error: Invalid cropped image dimensions." << std::endl;
        return *image; // Return the original image if the cropped dimensions are invalid
    }

    std::vector<uint8_t> croppedData(width * height * numChannels);

    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            int originalIndex = (y * width + x) * numChannels;
            int croppedIndex = (y * width + x) * numChannels;
            croppedData[croppedIndex + 0] = image->imageData[originalIndex * numChannels + 0];
            croppedData[croppedIndex + 1] = image->imageData[originalIndex * numChannels + 1];
            croppedData[croppedIndex + 2] = image->imageData[originalIndex * numChannels + 2];
        }
    }

    cv::Mat cvImage(height, width, CV_8UC3, croppedData.data());
    cv::imshow("Cropped Image", cvImage);
    cv::waitKey(0);

    return snoutImage(width, height, numChannels, croppedData);
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
    cropImage(cornerOne, cornerTwo, image);
}