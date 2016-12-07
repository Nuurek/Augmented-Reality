#pragma once
#include <vector>
#include "opencv2/opencv.hpp"

#include "vector2f.h"


struct ARMarker {
	Vector2f a, b, c, d;

	std::vector<cv::Point2f> getVectorized();
};