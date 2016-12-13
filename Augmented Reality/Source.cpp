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

#define GLM_FORCE_RADIANS

const unsigned int FPS = 30;
const unsigned int FRAME_WIDTH = 640;
const unsigned int FRAME_HEIGHT = 480;
const char* WINDOW_NAME = "Camera capture";

const bool USE_CAMERA = true;
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
	PoseFinder::init();
	detector.setBuffer(&buffer);
	FrameDecorator decorator(BORDER_SIZE, REGION_SIZE, STEP_SIZE);
	bool isRunning=true;
	glfwSetTime(0);
	float x, y, z; z = x = y = 0;
	std::vector<glm::mat4> cameraMatrix;
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
			cameraMatrix.clear();
			for (auto& marker : detector.getARMarkers()) {
				auto imagePoints = marker.getVectorizedForOpenCV();
				int i = 0;
				char* str;
				for (auto point = imagePoints.begin(); point != imagePoints.end(); point++) {
					auto color = cv::Scalar(i * 255 / (imagePoints.size()+1), 55, 255 - i * 255 / (imagePoints.size()+1));
					cv::circle(frame, *point, 5, color, -1);
					cv::putText(frame, itoa(i, str, 10), *point, cv::FONT_HERSHEY_SCRIPT_SIMPLEX, 2, color, 3, 8);
					i++;
				}
				
				cameraMatrix.push_back(PoseFinder::findPose(imagePoints, PoseFinder::getExample3DPoints()));
				for (auto& imagePoint : PoseFinder::projectedPoints) {
					cv::circle(frame, imagePoint, 5, CV_RGB(150, 0, 255), -1);
				}
			}
			
		}
		else {
			cameraMatrix.clear();
			cameraMatrix.push_back(glm::lookAt( //Wylicz macierz widoku
				glm::vec3(0.0f, 0.0f, 5.0f),
				glm::vec3(0.0f, 0.0f, 0.0f),
				glm::vec3(0.0f, 1.0f, 0.0f)));

			cameraMatrix.push_back(glm::lookAt( //Wylicz macierz widoku
				glm::vec3(1.0f, 1.0f, 5.0f),
				glm::vec3(0.0f, 1.0f, 0.0f),
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
		

		//cv::imshow(WINDOW_NAME, frame);
		glfwSetTime(0);
		glm::mat4 camRot = mat4(1.0f);
		camRot = glm::rotate(camRot, radians(x), glm::vec3(1, 0, 0));
		camRot = glm::rotate(camRot, radians(y), glm::vec3(0, 1, 0));
		camRot = glm::rotate(camRot, radians(z), glm::vec3(0, 0, 1));
		for (auto cameraMat = cameraMatrix.begin(); cameraMat != cameraMatrix.end(); cameraMat++) {
			*cameraMat = camRot * (*cameraMat);
		}
		for (auto point = PoseFinder::axeis2DPoints.begin(); point != PoseFinder::axeis2DPoints.end(); point += 5) {

			cv::line(frame, *point, *(point+1), cv::Scalar(0, 0, 255), 3);
			cv::line(frame, *point, *(point + 2), cv::Scalar(0, 255, 0), 3);
			cv::line(frame, *point, *(point + 3), cv::Scalar(255, 0, 255), 3);
			cv::line(frame, *point, *(point+4), cv::Scalar(255, 0, 0), 3);
		}
		if (WRITE_VIDEO) {
			videoWriter.write(frame);
		}
		PoseFinder::axeis2DPoints.clear();
		drawer.drawScene(&frame, cameraMatrix);
		keyManager.handleEvents();
	}

	return EXIT_SUCCESS;
}