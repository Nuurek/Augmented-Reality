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
const size_t FRAMES_PER_CALIBRATION = 5;
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
	CameraCalibrator cameraCalibrator(FRAMES_PER_CALIBRATION, CALIBRATION_DELAY);

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
	float x, y, z; z = x = y = 0;
	float angle = 0;
	std::vector<glm::mat4> cameraMatrix;
	while (!glfwWindowShouldClose(drawer.getWindow())) {
		if (USE_CAMERA) {
			camera >> frame;
		} else {
			frame = exampleImage.clone();
		}

		if (CALIBRATION) {
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

					return EXIT_SUCCESS;
				}
			}
		} else {
			buffer.setFrame(frame);

			detector.findARMarkers();

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

			cameraMatrix.clear();

			if (keyManager.isActive("poseFinderExample")) {
				auto markers = detector.getARMarkers();
				auto numberOfMarkers = markers.size();

				if (!markers.size()) {
					continue;
				}

				auto objectsPointsPatterns = std::vector<std::vector<cv::Point3f>>(numberOfMarkers, PoseFinder::getBottomOfTheCube3DPoints());
				auto imagePointsPatterns = std::vector<std::vector<cv::Point2f>>();

				for (auto& marker : markers) {
					auto imagePoints = marker.getVectorizedForOpenCV();
					imagePointsPatterns.push_back(imagePoints);
				}

				CameraCalibration cameraCalibration = poseFinder.calibrateCamera(objectsPointsPatterns, imagePointsPatterns);
				std::cout << cameraCalibration.cameraMatrix << "\n";
				std::cout << cameraCalibration.distCoeffs << "\n";
				auto focalX = cameraCalibration.cameraMatrix.at<double>(0, 0);
				if (focalX > frame.cols * 2.0f) {
					std::cout << "Camera calibration not found.\n";
					continue;
				}

				for (auto& marker : markers) {
					auto imagePoints = marker.getVectorizedForOpenCV();

					for (auto& imagePoint : imagePoints) {
						cv::circle(frame, imagePoint, 5, CV_RGB(0, 0, 255), -1);
					}
				}

				for (auto& marker : markers) {
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

			} else {
				cameraMatrix.push_back(glm::lookAt( //Wylicz macierz widoku
					glm::vec3(5.0f*cos(angle), 5.0f*sin(angle), 0.0f),
					glm::vec3(0.0f, 0.0f, 0.0f),
					glm::vec3(0.0f, 1.0f, 0.0f)));
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

			angle += glfwGetTime();
			glfwSetTime(0);
			glm::mat4 camRot = mat4(1.0f);
			camRot = glm::rotate(camRot, radians(x), glm::vec3(1, 0, 0));
			camRot = glm::rotate(camRot, radians(y), glm::vec3(0, 1, 0));
			camRot = glm::rotate(camRot, radians(z), glm::vec3(0, 0, 1));
			for (auto cameraMat = cameraMatrix.begin(); cameraMat != cameraMatrix.end(); cameraMat++) {
				*cameraMat = camRot * (*cameraMat);
			}
		}
		
		drawer.drawScene(&frame, cameraMatrix);

		if (WRITE_VIDEO) {
			videoWriter.write(frame);
		}

		cv::waitKey(10);
		keyManager.handleEvents();
	}

	return EXIT_SUCCESS;
}