#include "PoseFinder.h"
#include <memory>
#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/calib3d/calib3d.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "glm/gtx/string_cast.hpp"

PoseFinder::PoseFinder(const size_t borderSize, const size_t regionSize, const size_t stepSize)
	: RegionBasedOperator(borderSize, regionSize, stepSize) {
}


CameraCalibration PoseFinder::calibrateCamera(std::vector<VectorOf3DPoints> objectPointsPatterns, std::vector<VectorOf2DPoints> imagePointsPatterns) {
	CameraCalibration cameraCalibration(buffer->getWidth(), buffer->getHeight());
	//CameraCalibration cameraCalibration;
	
	cv::Mat rotationVector;
	cv::Mat translationVector;

	cv::calibrateCamera(objectPointsPatterns, imagePointsPatterns, cv::Size(buffer->getWidth(), buffer->getHeight()),
		cameraCalibration.cameraMatrix, cameraCalibration.distCoeffs,
		rotationVector, translationVector, CV_CALIB_USE_INTRINSIC_GUESS);
	
	if (numberOfCalibrations < maxNumberOfCalibrations) {
		++numberOfCalibrations;
	}

	float ratio = static_cast<float>(numberOfCalibrations - 1) / static_cast<float>(numberOfCalibrations);

	cv::addWeighted(averageCameraCalibration.cameraMatrix, ratio, cameraCalibration.cameraMatrix, (1.0 - ratio), 0.0, averageCameraCalibration.cameraMatrix, CV_32F);
	cv::addWeighted(averageCameraCalibration.distCoeffs, ratio, cameraCalibration.distCoeffs, (1.0 - ratio), 0.0, averageCameraCalibration.distCoeffs, CV_32F);

	return averageCameraCalibration;
}

TransformationMatrix PoseFinder::findTransformaton(VectorOf3DPoints objectPoints, VectorOf2DPoints imagePoints, CameraCalibration cameraCalibration) {
	TransformationMatrix transformationMatrix;

	cv::solvePnP(objectPoints, imagePoints, cameraCalibration.cameraMatrix,
		cameraCalibration.distCoeffs, transformationMatrix.rotationVector, transformationMatrix.translationVector);

	return transformationMatrix;
}

VectorOf2DPoints PoseFinder::getProjectedPoints(CameraCalibration cameraCalibration, TransformationMatrix transformationMatrix, VectorOf3DPoints objectPoints)
{
	VectorOf2DPoints projectedPoints;

	cv::projectPoints(objectPoints, transformationMatrix.rotationVector, transformationMatrix.translationVector,
		cameraCalibration.cameraMatrix, cameraCalibration.distCoeffs, projectedPoints);

	return projectedPoints;
}


std::vector<cv::Point3f> PoseFinder::getBottomOfTheCube3DPoints() {
	return std::vector<cv::Point3f>{ {-1.0, -1.0, -1.0}, { -1.0, 1.0, -1.0 }, { 1.0, 1.0, -1.0 }, { 1.0, -1.0, -1.0 }};
}

std::vector<cv::Point3f> PoseFinder::getTopOfTheCube3DPoints() {
	return std::vector<cv::Point3f>{ {-1.0, -1.0, 1.0}, { -1.0, 1.0, 1.0 }, { 1.0, 1.0, 1.0 }, { 1.0, -1.0, 1.0 }};
}

glm::mat4 TransformationMatrix::getViewMatrix() {
	cv::Mat viewMatrix = cv::Mat::zeros(4, 4, CV_32F);
	
	translationVector.copyTo(viewMatrix(cv::Rect(3, 0, 1, 3)));

	cv::Mat rotationMatrix;
	cv::Rodrigues(rotationVector, rotationMatrix);

	rotationMatrix.copyTo(viewMatrix(cv::Rect(0, 0, 3, 3)));
	viewMatrix.at<float>(3, 3) = 1.0f;

	cv::Mat invertYandZMatrix = cv::Mat::zeros(4, 4, CV_32F);
	invertYandZMatrix.at<float>(0, 0) = 1.0f;
	invertYandZMatrix.at<float>(1, 1) = -1.0f;
	invertYandZMatrix.at<float>(2, 2) = -1.0f;
	invertYandZMatrix.at<float>(3, 3) = 1.0f;

	viewMatrix = invertYandZMatrix * viewMatrix;

	cv::transpose(viewMatrix, viewMatrix);

	glm::mat4 openGLViewMatrix;

	for (size_t row = 0; row < 4; row++) {
		for (size_t column = 0; column < 4; column++) {
			openGLViewMatrix[row][column] = viewMatrix.at<float>(row, column);
		}
	}

	return openGLViewMatrix;
}

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
	cameraMatrix = cv::Mat::zeros(cv::Size(3, 3), CV_32F);
	distCoeffs = cv::Mat::zeros(cv::Size(1, 5), CV_32F);
}
