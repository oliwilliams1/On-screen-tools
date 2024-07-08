#pragma once
#include <vector>
#include <memory>
#include <opencv2/opencv.hpp>
#include <tesseract/baseapi.h>

class ocrHandler
{
private:
	tesseract::TessBaseAPI* tessApi;
public:
	ocrHandler();
	~ocrHandler();

	void ocrImageData(std::vector<uint8_t>* imageData);
};

