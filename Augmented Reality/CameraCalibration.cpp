#include "CameraCalibration.h"
#include "opencv2/opencv.hpp"


CameraCalibration::CameraCalibration(float width, float height) {
	double focalLength = width; // Approximate focal length.
	cv::Point2d center = cv::Point2d(width / 2.0f, height / 2.0f);
	cameraMatrix = cv::Mat::zeros(3, 3, CV_32F);
	cameraMatrix.at<float>(0, 0) = focalLength;
	cameraMatrix.at<float>(1, 1) = focalLength;

	cameraMatrix.at<float>(0, 2) = center.x;
	cameraMatrix.at<float>(1, 2) = center.y;

	cameraMatrix.at<float>(2, 2) = 1.0;

	distCoeffs = cv::Mat::zeros(5, 1, CV_32F); // Assuming no lens distortion
}

CameraCalibration::CameraCalibration() {
	cameraMatrix = cv::Mat::eye(3, 3, CV_32F);
	distCoeffs = cv::Mat::zeros(8, 1, CV_32F);
}
