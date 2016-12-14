#pragma once
#include <string>
#include "opencv2/opencv.hpp"


int FPS;
int FRAME_WIDTH;
int FRAME_HEIGHT;
std::string WINDOW_NAME;

bool USE_CAMERA = true;
std::string EXAMPLE_IMAGE_FILENAME;

bool WRITE_VIDEO;
std::string WRITE_FILENAME;

bool CALIBRATION;
int FRAMES_PER_CALIBRATION;
float CALIBRATION_DELAY;

int BORDER_SIZE;
int REGION_SIZE;
int STEP_SIZE;


void loadConfig(std::string filename) {
	cv::FileStorage fileStorage(filename, cv::FileStorage::READ);

	fileStorage["FPS"] >> FPS;
	fileStorage["FRAME_WIDTH"] >> FRAME_WIDTH;
	fileStorage["FRAME_HEIGHT"] >> FRAME_HEIGHT;
	fileStorage["WINDOW_NAME"] >> WINDOW_NAME;
	fileStorage["USE_CAMERA"] >> USE_CAMERA;
	fileStorage["EXAMPLE_IMAGE_FILENAME"] >> EXAMPLE_IMAGE_FILENAME;
	fileStorage["WRITE_VIDEO"] >> WRITE_VIDEO;
	fileStorage["WRITE_FILENAME"] >> WRITE_FILENAME;
	fileStorage["FRAMES_PER_CALIBRATION"] >> FRAMES_PER_CALIBRATION;
	fileStorage["CALIBRATION_DELAY"] >> CALIBRATION_DELAY;
	fileStorage["BORDER_SIZE"] >> BORDER_SIZE;
	fileStorage["REGION_SIZE"] >> REGION_SIZE;
	fileStorage["STEP_SIZE"] >> STEP_SIZE;

	fileStorage.release();
}