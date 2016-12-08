#include "ARMarker.h"


std::vector<cv::Point2f> ARMarker::getVectorized() {
	return std::vector<cv::Point2f>{
		cv::Point2f(vertices[0].x, vertices[0].y),
		cv::Point2f(vertices[1].x, vertices[1].y),
		cv::Point2f(vertices[2].x, vertices[2].y),
		cv::Point2f(vertices[3].x, vertices[3].y)
	};
}

ARMarker::ARMarker(Vector2f newVertices[4]) {
	for (int i = 0; i < 4; i++) {
		vertices[i] = newVertices[i];
	}
}
