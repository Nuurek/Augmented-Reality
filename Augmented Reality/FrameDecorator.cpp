#include "FrameDecorator.h"


FrameDecorator::FrameDecorator(const size_t borderSize, const size_t regionSize, const size_t stepSize)
	: RegionBasedOperator(borderSize, regionSize, stepSize) {}


void FrameDecorator::drawRegionLines(cv::Mat& frame) {
	for (int x = BORDER_SIZE; x < frame.cols; x += REGION_SIZE) {
		for (int y = BORDER_SIZE; y < frame.rows; y += REGION_SIZE) {
			cv::Rect region(x, y, x + REGION_SIZE - 1, y + REGION_SIZE - 1);

			cv::Scalar greenColor(0, 128, 0);

			cv::rectangle(frame, region, greenColor, 2);
		}
	}
}

void FrameDecorator::drawSubRegionLines(cv::Mat& frame) {
	for (int regionLeft = BORDER_SIZE; regionLeft < frame.cols; regionLeft += REGION_SIZE) {
		for (int regionTop = BORDER_SIZE; regionTop < frame.rows; regionTop += REGION_SIZE) {
			for (int x = 0; x < REGION_SIZE; x += STEP_SIZE) {
				for (int y = 0; y < REGION_SIZE; y += STEP_SIZE) {
					cv::Rect subregion(regionLeft + x, regionTop + y, 
						regionLeft + x + STEP_SIZE - 1, regionTop + y + STEP_SIZE - 1);

					cv::Scalar greenColor(128, 128, 128);

					cv::rectangle(frame, subregion, greenColor);
				}
			}
		}
	}
}

void FrameDecorator::drawEdgels(cv::Mat & frame, std::vector<Edgel> edgels) {
	for (auto& edgel : edgels) {
		drawEdgel(frame, edgel);
	}
}

void FrameDecorator::drawMagicEdgels(cv::Mat & frame, std::vector<std::pair<Edgel, double>> magicEdgels) {
	for (auto& magicEdgel : magicEdgels) {
		auto& edgel = magicEdgel.first;
		auto time = static_cast<double>(magicEdgel.second);
		cv::Point center(edgel.position.x, edgel.position.y);

		cv::Scalar color(randomColor(randomGenerator), randomColor(randomGenerator), randomColor(randomGenerator), 255.0 * time);

		cv::circle(frame, center, 1, color, 2.0 * time);
	}
}

void FrameDecorator::drawEdgel(cv::Mat & frame, Edgel & edgel) {
	cv::Point center(edgel.position.x, edgel.position.y);

	cv::Scalar blueColor(128, 0, 0);

	cv::circle(frame, center, 1, blueColor, 2);
}