#include "Buffer.h"


void Buffer::setFrame(cv::Mat& frame) {
	width = frame.cols;
	height = frame.rows;
	data = frame.data;
}

Buffer::operator std::string() {
	return std::to_string(width) + " x " + std::to_string(height);
}
