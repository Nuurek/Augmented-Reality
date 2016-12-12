#include "opencv2/opencv.hpp"
#include <iostream>
#include <string>
#include <chrono>
#include "PoseFinder.h"
#include "Drawer.h"
#include "Buffer.h"
#include "ARMarkerDetector.h"
#include "KeyManager.h"
#include "FrameDecorator.h"
#include "PoseFinder.h"

#define GLM_FORCE_RADIANS

const unsigned int FPS = 30;
const unsigned int FRAME_WIDTH = 640;
const unsigned int FRAME_HEIGHT = 480;
const char* WINDOW_NAME = "Camera capture";

const bool USE_CAMERA = false;
const char* EXAMPLE_IMAGE_FILENAME = "example.jpg";

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
	cv::VideoCapture camera;
	cv::Mat exampleImage;

	if (USE_CAMERA) {
		camera = cv::VideoCapture(0);

		if (!camera.isOpened()) {
			return exitWithError("Couldn't open camera.");
		}
	} else {
		exampleImage = cv::imread(EXAMPLE_IMAGE_FILENAME);
		cv::resize(exampleImage, exampleImage, cv::Size(FRAME_WIDTH, FRAME_HEIGHT));
	}

	//cv::namedWindow(WINDOW_NAME);
	Drawer drawer;
	drawer.init(FRAME_WIDTH,FRAME_HEIGHT);

	auto videoWriter = cv::VideoWriter();
	if (WRITE_VIDEO) {
		videoWriter.open(
			WRITE_FILENAME, cv::VideoWriter::fourcc('P', 'I', 'M', '1'), static_cast<double>(FPS), cv::Size(FRAME_WIDTH, FRAME_HEIGHT)
		);

		if (!videoWriter.isOpened()) {
			return exitWithError("Couldn't create video writer.");
		}
	}

	cv::Mat frame;
	Buffer buffer;
	ARMarkerDetector detector(BORDER_SIZE, REGION_SIZE, STEP_SIZE);

	KeyManager keyManager;
	keyManager.init(drawer.getWindow());

	detector.setBuffer(&buffer);
	FrameDecorator decorator(BORDER_SIZE, REGION_SIZE, STEP_SIZE);
	bool isRunning=true;

	PoseFinder poseFinder(BORDER_SIZE, REGION_SIZE, STEP_SIZE);
	poseFinder.setBuffer(&buffer);

	glfwSetTime(0);
	glm::mat4 cameraMatrix = glm::mat4(1.0f);
	while (!glfwWindowShouldClose(drawer.getWindow())) {
		if (USE_CAMERA) {
			camera >> frame;
		} else {
			frame = exampleImage.clone();
		}
		
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

		if (keyManager.isActive("mergedLineSegments")) {
			auto mergedLineSegments = detector.getMergedLineSegments();
			decorator.drawLineSegments(frame, mergedLineSegments);
		}

		if (keyManager.isActive("extendedLineSegments")) {
			auto extendedLineSegments = detector.getExtendedLineSegments();
			decorator.drawLineSegments(frame, extendedLineSegments);
		}

		if (keyManager.isActive("lineSegmentsWithCorner")) {
			auto lineSegmentsWithCorner = detector.getLineSegmentsWithCorner();
			decorator.drawLineSegments(frame, lineSegmentsWithCorner);
		}

		if (keyManager.isActive("markers")) {
			auto markers = detector.getARMarkers();
			decorator.drawARMarkers(frame, markers);
		}
		if (keyManager.isActive("poseFinderExample")) {
			auto markers = detector.getARMarkers();
			auto numberOfMarkers = markers.size();
			auto objectsPointsPatterns = std::vector<std::vector<cv::Point3f>>(numberOfMarkers, PoseFinder::getBottomOfTheCube3DPoints());
			auto imagePointsPatterns = std::vector<std::vector<cv::Point2f>>();

			for (auto& marker : markers) {
				auto imagePoints = marker.getVectorizedForOpenCV();
				imagePointsPatterns.push_back(imagePoints);

				for (auto& imagePoint : imagePoints) {
					cv::circle(frame, imagePoint, 5, CV_RGB(0, 0, 255), -1);
				}
			}

			auto cameraCalibration = poseFinder.calibrateCamera(objectsPointsPatterns, imagePointsPatterns);

			for (auto& marker : markers) {
				auto bottomImagePoints = marker.getVectorizedForOpenCV();
				auto topImagePoints = poseFinder.findPose(cameraCalibration, PoseFinder::getBottomOfTheCube3DPoints(), bottomImagePoints);
				for (auto& imagePoint : topImagePoints) {
					cv::circle(frame, imagePoint, 5, CV_RGB(255, 0, 0), -1);
				}
			}
		}
		else {
			cameraMatrix = glm::lookAt( //Wylicz macierz widoku
				glm::vec3(2.0f, 2.0f, 2.0f),
				glm::vec3(0.0f, 0.0f, 0.0f),
				glm::vec3(-1.0f, -1.0f, 1.0f));
		}

		if (keyManager.isActive("escape")) 
			glfwSetWindowShouldClose(drawer.getWindow(), GLFW_TRUE);

		if (WRITE_VIDEO) {
			videoWriter.write(frame);
		}

		//cv::imshow(WINDOW_NAME, frame);
		glfwSetTime(0);
		drawer.drawScene(&frame, cameraMatrix);
		keyManager.handleEvents();
	}

	return EXIT_SUCCESS;
}