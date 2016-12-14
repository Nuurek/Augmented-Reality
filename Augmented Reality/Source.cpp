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
#include "CameraCalibrator.h"
#include "ProgramMode.h"

#define GLM_FORCE_RADIANS

const unsigned int FPS = 30;
const unsigned int FRAME_WIDTH = 640;
const unsigned int FRAME_HEIGHT = 480;
const char* WINDOW_NAME = "Camera capture";

const bool USE_CAMERA = true;
const char* EXAMPLE_IMAGE_FILENAME = "example.jpg";

const bool WRITE_VIDEO = true;
const char* WRITE_FILENAME = "capture.mpeg";

const bool CALIBRATION = true;
const size_t FRAMES_PER_CALIBRATION = 30;
const float CALIBRATION_DELAY = 1.0f;

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

	cv::VideoWriter videoWriter;
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
	detector.setBuffer(&buffer);
	
	FrameDecorator decorator(BORDER_SIZE, REGION_SIZE, STEP_SIZE);

	CameraCalibrator cameraCalibrator(FRAMES_PER_CALIBRATION, CALIBRATION_DELAY);
	CameraCalibration cameraCalibration;
	cameraCalibration = cameraCalibrator.loadFromFile("camera-calibration.xml");

	PoseFinder poseFinder;

	Drawer drawer;
	drawer.init(FRAME_WIDTH, FRAME_HEIGHT);
	KeyManager keyManager(drawer.getWindow());

	glfwSetTime(0);
	float x, y, z; z = x = y = 0;
	float angle = 0;
	std::vector<glm::mat4> cameraMatrix;

	Mode mode = Mode::IDLE;

	while (!glfwWindowShouldClose(drawer.getWindow())) {
		if (USE_CAMERA) {
			camera >> frame;
		} else {
			frame = exampleImage.clone();
		}

		cameraMatrix.clear();

		if (mode != Mode::CALIBRATING) {
			if (keyManager.keyString == "calibrating") {
				mode = Mode::CALIBRATING;
			} else if (keyManager.keyString == "markerFinding") {
				mode = Mode::MARKER_FINDING;
			} else if (keyManager.keyString == "augmentedReality") {
				mode = Mode::AUGMENTED_REALITY;
			}
		}

		if (mode == Mode::CALIBRATING) {
			if (!cameraCalibrator.isSessionRunning()) {
				cameraCalibrator.setFrameSize(frame.rows, frame.cols);
				cameraCalibrator.startSession();
			} else {
				bool found = cameraCalibrator.addFrame(frame);

				if (found) {
					cv::bitwise_not(frame, frame);
				}

				if (cameraCalibrator.isSessionFinished()) {
					auto cameraCalibration = cameraCalibrator.getCameraCalibration();

					std::cout << "Calibration finished\n";
					std::cout << "Camera matrix: " << cameraCalibration.cameraMatrix << "\n";
					std::cout << "Distance coefficients: " << cameraCalibration.distCoeffs << "\n";

					cameraCalibrator.saveToFile(cameraCalibration, "camera-calibration.xml");

					mode = Mode::IDLE;
				}
			}

			if (!keyManager.isActive("calibrating")) {
				mode = Mode::IDLE;
			}
		}

		if (mode == Mode::MARKER_FINDING || mode == Mode::AUGMENTED_REALITY) {
			buffer.setFrame(frame);
			detector.findARMarkers();
		}

		if (mode == Mode::MARKER_FINDING) {
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

			if (!keyManager.isActive("markerFinding")) {
				mode = Mode::IDLE;
			}
		}

		if (mode == Mode::AUGMENTED_REALITY) {

			auto markers = detector.getARMarkers();
			auto numberOfMarkers = markers.size();

			for (auto& marker : markers) {
				auto imagePoints = marker.getVectorizedForOpenCV();

				for (auto& imagePoint : imagePoints) {
					cv::circle(frame, imagePoint, 5, CV_RGB(0, 0, 255), -1);
				}

				auto bottomImagePoints = marker.getVectorizedForOpenCV();
				auto bottomObjectPoints = PoseFinder::getBottomOfTheCube3DPoints();
				auto transformationMatrix = poseFinder.findTransformaton(bottomObjectPoints, bottomImagePoints, cameraCalibration);
				cameraMatrix.push_back(transformationMatrix.getViewMatrix());
				auto topObjectPoints = PoseFinder::getTopOfTheCube3DPoints();
				auto topImagePoints = poseFinder.getProjectedPoints(cameraCalibration, transformationMatrix, topObjectPoints);
				for (auto& imagePoint : topImagePoints) {
					cv::circle(frame, imagePoint, 5, CV_RGB(255, 0, 0), -1);
				}
			}

			for (auto& marker : markers) {
					
			}

			if (keyManager.isActive("escape"))
				glfwSetWindowShouldClose(drawer.getWindow(), GLFW_TRUE);

			if (keyManager.isActive("W")) {
				y += 15;
				keyManager.keyPressed(GLFW_KEY_W);
			}

			if (keyManager.isActive("S")) {
				y -= 15;
				keyManager.keyPressed(GLFW_KEY_S);
			}

			if (keyManager.isActive("A")) {
				x += 15;
				keyManager.keyPressed(GLFW_KEY_A);
			}

			if (keyManager.isActive("D")) {
				x -= 15;
				keyManager.keyPressed(GLFW_KEY_D);
			}

			if (keyManager.isActive("E")) {
				z += 15;
				keyManager.keyPressed(GLFW_KEY_E);
			}

			if (keyManager.isActive("Q")) {
				z -= 15;
				keyManager.keyPressed(GLFW_KEY_Q);
			}

			if (!keyManager.isActive("augmentedReality")) {
				mode = Mode::IDLE;
			}
		}

		decorator.drawMode(frame, mode);

		drawer.drawScene(&frame, cameraMatrix);

		if (WRITE_VIDEO) {
			videoWriter.write(frame);
		}

		if (keyManager.isActive("escape")) {
			glfwSetWindowShouldClose(drawer.getWindow(), true);
		}

		cv::waitKey(10);
		keyManager.handleEvents();
	}

	return EXIT_SUCCESS;
}