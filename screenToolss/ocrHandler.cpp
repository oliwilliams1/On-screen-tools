#include "ocrHandler.h"

ocrHandler::ocrHandler()
{
	tessApi = new tesseract::TessBaseAPI();
	if (tessApi->Init(nullptr, "eng")) {
		std::cerr << "Could not init tesseract api" << std::endl;
		exit(1);
	}
}

ocrHandler::~ocrHandler()
{
	tessApi->End();
	delete tessApi;
}

void ocrHandler::ocrImageData(std::vector<uint8_t>* imageData)
{
	int height = 1080;
	int width = 1920;
	int channels = 3;

	cv::Mat image(height, width, CV_8UC3, imageData->data());

	for (int i = 0; i < image.total() * image.channels(); i += image.channels()) {
		std::swap(image.data[i], image.data[i + 2]);
		/* Swaps colour channels because imageData is captured backwards or something 
		   | B | G | R | (backwards)
		   | R | G | B | (desired)
		     ^--<->--^
		*/
	}

	tessApi->SetImage(image.data, image.cols, image.rows, image.channels(), static_cast<int>(image.step));

	char* text = tessApi->GetUTF8Text();
	
	std::cout << "Recognized text: " << text << std::endl;

	cv::imshow("Image", image);
	cv::waitKey(0);

	delete text;
}