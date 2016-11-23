#pragma once
#include "opencv2\opencv.hpp"
#include <vector>
#include <utility>
#include <random>

#include "RegionBasedOperator.h"
#include "Edgel.h"


class FrameDecorator : RegionBasedOperator {
	void drawEdgel(cv::Mat& frame, Edgel& edgel);

	std::mt19937 randomGenerator{};
	std::uniform_int_distribution<> randomColor{ 0, 256 };
public:
	FrameDecorator(const size_t borderSize, const size_t regionSize, const size_t stepSize);

	void drawRegionLines(cv::Mat& frame);
	void drawSubRegionLines(cv::Mat& frame);
	void drawEdgels(cv::Mat& frame, std::vector<Edgel> edgels);
	void drawMagicEdgels(cv::Mat& frame, std::vector<std::pair<Edgel, double>> magicEdgels);
};

