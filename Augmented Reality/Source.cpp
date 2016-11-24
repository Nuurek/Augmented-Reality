#include "opencv2/opencv.hpp"
#include <iostream>
#include <string>
#include <chrono>

#include "Buffer.h"
#include "ARMarkerDetector.h"
#include "KeyManager.h"
#include "FrameDecorator.h"


const unsigned int FPS = 30;
const unsigned int FRAME_WIDTH = 640;
const unsigned int FRAME_HEIGHT = 480;
const char* WINDOW_NAME = "Camera capture";

const bool WRITE_VIDEO = true;
const char* WRITE_FILENAME = "capture.mpeg";

const size_t BORDER_SIZE = 2;
const size_t REGION_SIZE = 40;
const size_t STEP_SIZE = 5;

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

	cv::Mat frame;
	Buffer buffer;
	ARMarkerDetector detector(BORDER_SIZE, REGION_SIZE, STEP_SIZE);

	KeyManager keyManager;

	detector.setBuffer(&buffer);
	FrameDecorator decorator(BORDER_SIZE, REGION_SIZE, STEP_SIZE);

	while (true) {
		camera >> frame;
		
		buffer.setFrame(frame);

		auto start = std::chrono::high_resolution_clock::now();
		detector.findARMarkers();
		auto end = std::chrono::high_resolution_clock::now();
		auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
		std::cout << duration << "us\n";

		if (keyManager.isActive("regions")) {
			decorator.drawRegionLines(frame);
		}

		if (keyManager.isActive("subRegions")) {
			decorator.drawSubRegionLines(frame);
		}

		if (keyManager.isActive("edgels")) {
			auto edgels = detector.getEdgels();
			decorator.drawEdgels(frame, edgels);
		}

		if (keyManager.isActive("lineSegments")) {
			auto lineSegments = detector.getLineSegments();
			decorator.drawLineSegments(frame, lineSegments);
		}

		if (WRITE_VIDEO) {
			videoWriter.write(frame);
		}

		cv::imshow(WINDOW_NAME, frame);
		auto keyCode = cv::waitKey(1000 / FPS);
		if (keyCode == 'q') {
			return EXIT_SUCCESS;
		} else {
			keyManager.keyPressed(keyCode);
		}
	}
}