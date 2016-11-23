#pragma once
#include "opencv2\opencv.hpp"

#include "RegionBasedOperator.h"


class FrameDecorator : RegionBasedOperator {

public:
	FrameDecorator(const size_t borderSize, const size_t regionSize, const size_t stepSize);

	void drawRegionLines(cv::Mat& frame);
	void drawSubRegionLines(cv::Mat& frame);
};

