#include "opencv2/opencv.hpp"


#include <iostream>

const unsigned int FPS = 30;
const unsigned int FRAME_WIDTH = 640;
const unsigned int FRAME_HEIGHT = 480;
const char * WINDOW_NAME = "Camera capture";

const bool WRITE_VIDEO = true;
const char * WRITE_FILENAME = "capture.mpeg";


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

	auto videoWriter = cv::VideoWriter();
	if (WRITE_VIDEO) {
		videoWriter.open(
			WRITE_FILENAME, cv::VideoWriter::fourcc('P', 'I', 'M', '1'), FPS, cv::Size(FRAME_WIDTH, FRAME_HEIGHT)
		);

		if (!videoWriter.isOpened()) {
			return exitWithError("Couldn't create video writer.");
		}
	}

	while (true) {
		cv::Mat frame;
		camera >> frame;

		if (WRITE_VIDEO) {
			videoWriter.write(frame);
		}

		cv::imshow(WINDOW_NAME, frame);
		if (cv::waitKey(1000 / FPS) >= 0) {
			return EXIT_SUCCESS;
		}
	}
}