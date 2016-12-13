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

ViewMatrix PoseFinder::findTransformaton(VectorOf3DPoints objectPoints, VectorOf2DPoints imagePoints, CameraCalibration cameraCalibration) {
	ViewMatrix transformationMatrix;

	cv::solvePnPRansac(objectPoints, imagePoints, cameraCalibration.cameraMatrix,
		cameraCalibration.distCoeffs, transformationMatrix.rotationVector, transformationMatrix.translationVector);

	return transformationMatrix;
}

VectorOf2DPoints PoseFinder::getProjectedPoints(CameraCalibration cameraCalibration, ViewMatrix transformationMatrix, VectorOf3DPoints objectPoints)
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