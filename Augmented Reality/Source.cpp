#include "opencv2/opencv.hpp"


#include <iostream>

const unsigned int FPS = 30;
const unsigned int FRAME_WIDTH = 640;
const unsigned int FRAME_HEIGHT = 480;
const char * WINDOW_NAME = "Camera capture";


int exitWithError(const char * errorMessage) {
	std::cerr << errorMessage << "\n";
	return EXIT_FAILURE;
}

int main(int argc, char** argv) {
	auto camera = cv::VideoCapture(0);

	if (!camera.isOpened()) {
		return exitWithError("Couldn't open camera.");
	}

	cv::namedWindow(WINDOW_NAME);

	while (true) {
		cv::Mat frame;
		camera >> frame;
		cv::imshow(WINDOW_NAME, frame);
		if (cv::waitKey(1) >= 0) {
			return EXIT_SUCCESS;
		}
	}
}