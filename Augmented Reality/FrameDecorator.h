#pragma once
#include "opencv2\opencv.hpp"
#include <vector>

#include "RegionBasedOperator.h"
#include "Edgel.h"


class FrameDecorator : RegionBasedOperator {
	void drawEdgel(cv::Mat& frame, Edgel& edgel);
public:
	FrameDecorator(const size_t borderSize, const size_t regionSize, const size_t stepSize);

	void drawRegionLines(cv::Mat& frame);
	void drawSubRegionLines(cv::Mat& frame);
	void drawEdgels(cv::Mat& frame, std::vector<Edgel> edgels);
};

