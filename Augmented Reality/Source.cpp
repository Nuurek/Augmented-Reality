#include "opencv2/opencv.hpp"
#include <iostream>
#include <string>
#include <chrono>

#include "Config.h"
#include "PoseFinder.h"
#include "Drawer.h"
#include "Buffer.h"
#include "ARMarkerDetector.h"
#include "KeyManager.h"
#include "FrameDecorator.h"
#include "CameraCalibrator.h"
#include "ProgramMode.h"
#include "PatternRecognition.h"
#define GLM_FORCE_RADIANS

int exitWithError(const char * errorMessage) {
	std::cerr << errorMessage << "\n";
	return EXIT_FAILURE;
}

int main(int argc, char** argv) {
	loadConfig("config.xml");

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
	PatternRecognition::init();
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
	int frameTimeInMS = 1000 / FPS;

	std::vector<int> objectIds;

	while (!glfwWindowShouldClose(drawer.getWindow())) {
		auto start = std::chrono::steady_clock::now();

		if (USE_CAMERA) {
			camera >> frame;
		} else {
			frame = exampleImage.clone();
		}
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

				if (keyManager.isActive("cornersDebug")) {
					std::cout << "Debug corners\n";
					for (auto& imagePoint : imagePoints) {
						cv::circle(frame, imagePoint, 5, CV_RGB(0, 0, 255), -1);
					}
				}

				unsigned int patternId = PatternRecognition::getPatternId(frame, imagePoints);
				if (patternId == INT_MAX) {
					cv::putText(frame, "Nope", imagePoints[3], cv::FONT_HERSHEY_SIMPLEX, 1, CV_RGB(50, 50, 50), 2, 8);
					continue;
				}
				int orientation = patternId % 10;
				objectIds.push_back(patternId / 10);
				cv::putText(frame, std::to_string(patternId), imagePoints[3], cv::FONT_HERSHEY_SIMPLEX, 1, CV_RGB(250, 150, 0), 2, 8);
				auto bottomImagePoints = marker.getVectorizedForOpenCV();
				auto bottomObjectPoints = PoseFinder::getBottomOfTheCube3DPoints(orientation);
				auto transformationMatrix = poseFinder.findTransformaton(bottomObjectPoints, bottomImagePoints, cameraCalibration);
				cameraMatrix.push_back(transformationMatrix.getViewMatrix());
				auto topObjectPoints = PoseFinder::getTopOfTheCube3DPoints();
				auto topImagePoints = poseFinder.getProjectedPoints(cameraCalibration, transformationMatrix, topObjectPoints);
				for (auto& imagePoint : topImagePoints) {
					if (keyManager.isActive("cornersDebug")) {
						cv::circle(frame, imagePoint, 5, CV_RGB(255, 0, 0), -1);
					}
				}
			}

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

		drawer.drawScene(&frame, cameraMatrix, objectIds);

		if (WRITE_VIDEO) {
			videoWriter.write(frame);
		}

		if (keyManager.isActive("escape")) {
			glfwSetWindowShouldClose(drawer.getWindow(), true);
		}

		auto end = std::chrono::steady_clock::now();
		std::chrono::duration<float> duration = end - start;
		auto milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();

		auto timeToWait = frameTimeInMS - milliseconds;
		if (timeToWait < 1) {
			timeToWait = 1;
		}

		cv::waitKey(timeToWait);
		keyManager.handleEvents();
	}

	return EXIT_SUCCESS;
}