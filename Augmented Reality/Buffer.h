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
		x = (x < 0) ? 0 : x;
		x = (x < 0) ? 0 : x;

		x = (x > width - 1) ? (width - 1) : x;
		y = (y > height - 1) ? (height - 1) : y;

		return data[((x + (y * width)) * 3) + channel];
	}

	inline float getPixelChannel(float x, float y, size_t channel) {
		size_t intX = static_cast<size_t>(x);
		size_t intY = static_cast<size_t>(y);

		return getPixelChannel(intX, intY, channel);
	}

	inline float getPixel(size_t x, size_t y) {
		return getPixelChannel(x, y, 0);
	}

	inline float getPixel(float x, float y) {
		return getPixelChannel(x, y, 0);
	}

	operator std::string();
};

