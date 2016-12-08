#include "FrameDecorator.h"


FrameDecorator::FrameDecorator(const size_t borderSize, const size_t regionSize, const size_t stepSize)
	: RegionBasedOperator(borderSize, regionSize, stepSize) {}


void FrameDecorator::drawRegionLines(cv::Mat& frame) {
	for (size_t x = BORDER_SIZE; x < frame.cols; x += REGION_SIZE) {
		for (size_t y = BORDER_SIZE; y < frame.rows; y += REGION_SIZE) {
			cv::Rect region(static_cast<int>(x), static_cast<int>(y), 
				static_cast<int>(x + REGION_SIZE - 1), static_cast<int>(y + REGION_SIZE - 1));

			cv::rectangle(frame, region, greenColor, 2);
		}
	}
}

void FrameDecorator::drawSubRegionLines(cv::Mat& frame) {
	for (size_t regionLeft = BORDER_SIZE; regionLeft < frame.cols; regionLeft += REGION_SIZE) {
		for (size_t regionTop = BORDER_SIZE; regionTop < frame.rows; regionTop += REGION_SIZE) {
			for (size_t x = 0; x < REGION_SIZE; x += STEP_SIZE) {
				for (size_t y = 0; y < REGION_SIZE; y += STEP_SIZE) {
					cv::Rect subregion(static_cast<int>(regionLeft + x), static_cast<int>(regionTop + y),
						static_cast<int>(regionLeft + x + STEP_SIZE - 1), 
						static_cast<int>(regionTop + y + STEP_SIZE - 1));

					cv::rectangle(frame, subregion, midGrayColor);
				}
			}
		}
	}
}

void FrameDecorator::drawEdgels(cv::Mat& frame, std::vector<Edgel> edgels) {
	for (auto& edgel : edgels) {
		drawEdgel(frame, edgel);
	}
}

void FrameDecorator::drawLineSegments(cv::Mat & frame, std::vector<LineSegment> lineSegments) {
	for (auto& lineSegment : lineSegments) {
		drawLineSegment(frame, lineSegment);
	}
}

void FrameDecorator::drawARMarkers(cv::Mat & frame, std::vector<ARMarker> markers) {
	for (auto& marker : markers) {
		drawARMarker(frame, marker);
	}
}

void FrameDecorator::drawEdgel(cv::Mat & frame, Edgel & edgel) {
	cv::Point center(static_cast<int>(edgel.position.x), static_cast<int>(edgel.position.y));
	auto red = 127.0 + edgel.slope.x * 128.0;
	auto green = 127.0 + edgel.slope.y * 128.0;
	cv::Scalar color{ 0.0, green, red };

	cv::circle(frame, center, 1, color, 2);
}

void FrameDecorator::drawLineSegment(cv::Mat & frame, LineSegment & lineSegment) {
	auto& start = lineSegment.start;
	auto& end = lineSegment.end;
	auto& slope = lineSegment.slope;

	drawLine(frame, start.position, end.position, yellowColor, 2);
	drawLine(frame, end.position, Vector2f(end.position.x + 5.0f * (-slope.x + slope.y), end.position.y + 5.0f * (-slope.y - slope.x)), redColor, 2);
	drawLine(frame, end.position, Vector2f(end.position.x + 5.0f * (-slope.x - slope.y), end.position.y + 5.0f * (-slope.y + slope.x)), redColor, 2);
}

void FrameDecorator::drawARMarker(cv::Mat & frame, ARMarker & marker) {
	auto& vertices = marker.vertices;
	auto size = vertices.size();
	for (int i = 0; i < size; i++) {
		auto& start = vertices[i];
		auto& end = vertices[(i + 1) % size];
		drawLine(frame, start, end, redColor, 2);
	}
}

void FrameDecorator::drawLine(cv::Mat& frame, Vector2f& start, Vector2f& end, cv::Scalar& color, int thickness) const {
	cv::Point startPoint(static_cast<int>(start.x), static_cast<int>(start.y));
	cv::Point endPoint(static_cast<int>(end.x), static_cast<int>(end.y));

	cv::line(frame, startPoint, endPoint, color, thickness);
}

