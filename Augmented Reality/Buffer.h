#pragma once
#include "opencv2\opencv.hpp"
#include <string>


class Buffer {
	unsigned int width;
	unsigned int height;
	unsigned char* data;

public:
	void setFrame(cv::Mat& frame);
	
	inline unsigned int getWidth() { return width; }
	inline unsigned int getHeight() { return height; }
	inline unsigned char* getData() { return data; }
	inline float getPixelChannel(float x, float y, size_t channel) {
		int intX = static_cast<int>(x);
		int intY = static_cast<int>(y);

		intX = (intX < 0) ? 0 : intX;
		intY = (intY < 0) ? 0 : intY;

		intX = (intX > width - 1) ? (width - 1) : intX;
		intY = (intY > height - 1) ? (height - 1) : intY;

		return data[((intX + (intY * width)) * 3) + channel];
	}
	inline float getPixel(float x, float y) {
		return getPixelChannel(x, y, 0);
	}

	operator std::string();
};

