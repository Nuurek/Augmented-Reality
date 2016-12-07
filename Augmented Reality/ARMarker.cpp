#include "ARMarker.h"


std::vector<cv::Point2f> ARMarker::getVectorized() {
	return std::vector<cv::Point2f>{
		cv::Point2f(a.x, a.y),
		cv::Point2f(b.x, b.y),
		cv::Point2f(c.x, c.y),
		cv::Point2f(d.x, d.y)
	};
}
