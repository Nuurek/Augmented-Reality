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
	inline float getPixelChannel(size_t x, size_t y, size_t channel) {
		return data[((x + (y * width)) * 3) + channel];
	}
	inline float getPixel(size_t x, size_t y) {
		return getPixelChannel(x, y, 0);
	}

	operator std::string();
};

