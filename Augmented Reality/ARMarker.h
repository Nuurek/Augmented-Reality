#pragma once
#include <vector>
#include <array>
#include "opencv2/opencv.hpp"

#include "vector2f.h"


struct ARMarker {
	std::array<Vector2f, 4> vertices;

	std::vector<cv::Point2f> getVectorizedForOpenCV();

	ARMarker(std::array<Vector2f, 4> newVertices);
	ARMarker(Vector2f vertices[4]);
};