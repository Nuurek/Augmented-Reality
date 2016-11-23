#include "FrameDecorator.h"



FrameDecorator::FrameDecorator(const size_t borderSize, const size_t regionSize, const size_t stepSize)
	: RegionBasedOperator(borderSize, regionSize, stepSize) {}


void FrameDecorator::drawRegionLines(cv::Mat& frame) {
	for (size_t x = BORDER_SIZE; x < frame.cols; x += REGION_SIZE) {
		for (size_t y = BORDER_SIZE; y < frame.rows; y += REGION_SIZE) {
			cv::Rect region(x, y, x + REGION_SIZE - 1, y + REGION_SIZE - 1);

			cv::Scalar greenColor(0, 128, 0);

			cv::rectangle(frame, region, greenColor, 2);
		}
	}
}

void FrameDecorator::drawSubRegionLines(cv::Mat& frame) {
	for (size_t regionLeft = BORDER_SIZE; regionLeft < frame.cols; regionLeft += REGION_SIZE) {
		for (size_t regionTop = BORDER_SIZE; regionTop < frame.rows; regionTop += REGION_SIZE) {
			for (size_t x = 0; x < REGION_SIZE; x += STEP_SIZE) {
				for (size_t y = 0; y < REGION_SIZE; y += STEP_SIZE) {
					cv::Rect subregion(regionLeft + x, regionTop + y, 
						regionLeft + x + STEP_SIZE - 1, regionTop + y + STEP_SIZE - 1);

					cv::Scalar greenColor(0, 64, 0);

					cv::rectangle(frame, subregion, greenColor);
				}
			}
		}
	}
}
