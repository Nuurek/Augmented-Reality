#pragma once
#include "opencv2/opencv.hpp"


struct CameraCalibration {
	cv::Mat cameraMatrix;
	cv::Mat distCoeffs;

	CameraCalibration(float width, float height);
	CameraCalibration();
};