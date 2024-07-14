struct snoutImage
{
	int width;
	int height;
	int channels;
	std::vector<uint8_t> imageData;

	snoutImage(int width, int height, int channels, std::vector<uint8_t> imageData)
	{
		this->width = width;
		this->height = height;
		this->channels = channels;
		this->imageData = imageData;
	}	
};