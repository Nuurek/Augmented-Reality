#pragma once
#include "opencv2\opencv.hpp"
#include <vector>
#include <unordered_map>

#include "RegionBasedOperator.h"
#include "Edgel.h"
#include "LineSegment.h"
#include "ARMarker.h"
#include "ProgramMode.h"


class FrameDecorator : public RegionBasedOperator {
	cv::Scalar blueColor{ 128, 0, 0 };
	cv::Scalar greenColor{ 0, 128, 0 };
	cv::Scalar redColor{ 0, 0, 128 };
	cv::Scalar midGrayColor{ 128, 128, 128 };
	cv::Scalar yellowColor{ 0, 128, 128 };

	void drawEdgel(cv::Mat& frame, Edgel& edgel);
	void drawLineSegment(cv::Mat& frame, LineSegment& lineSegment);
	void drawARMarker(cv::Mat& frame, ARMarker& marker);

	void drawLine(cv::Mat& frame, Vector2f& start, Vector2f& end, cv::Scalar& color, int thickness) const;

	static std::unordered_map<Mode, std::string> modeToStringMap;
public:
	FrameDecorator(const size_t borderSize, const size_t regionSize, const size_t stepSize);

	void drawRegionLines(cv::Mat& frame);
	void drawSubRegionLines(cv::Mat& frame);
	void drawEdgels(cv::Mat& frame, std::vector<Edgel> edgels);
	void drawLineSegments(cv::Mat& frame, std::vector<LineSegment> lineSegments);
	void drawARMarkers(cv::Mat& frame, std::vector<ARMarker> markers);
	void drawMode(cv::Mat& frame, Mode mode);
};