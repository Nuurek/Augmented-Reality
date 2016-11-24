#pragma once
#include "opencv2\opencv.hpp"
#include <vector>

#include "RegionBasedOperator.h"
#include "Edgel.h"
#include "LineSegment.h"


class FrameDecorator : RegionBasedOperator {
	cv::Scalar blueColor{ 128, 0, 0 };
	cv::Scalar greenColor{ 0, 128, 0 };
	cv::Scalar redColor{ 0, 0, 128 };
	cv::Scalar midGrayColor{ 128, 128, 128 };
	cv::Scalar yellowColor{ 0, 128, 128 };


	void drawEdgel(cv::Mat& frame, Edgel& edgel);
	void drawLineSegment(cv::Mat& frame, LineSegment& lineSegment);
public:
	FrameDecorator(const size_t borderSize, const size_t regionSize, const size_t stepSize);

	void drawRegionLines(cv::Mat& frame);
	void drawSubRegionLines(cv::Mat& frame);
	void drawEdgels(cv::Mat& frame, std::vector<Edgel> edgels);
	void drawLineSegments(cv::Mat& frame, std::vector<LineSegment> lineSegments);
};

