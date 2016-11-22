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

	operator std::string();
};

