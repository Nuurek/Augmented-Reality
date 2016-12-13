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


CameraCalibration PoseFinder::calibrateCamera(std::vector<VectorOf3DPoints> objectPointsPatterns, std::vector<VectorOf2DPoints> imagePointsPatters) {
	CameraCalibration cameraCalibration;

	cv::Mat rotationVector;
	cv::Mat translationVector;

	cv::calibrateCamera(objectPointsPatterns, imagePointsPatters, cv::Size(buffer->getWidth(), buffer->getHeight()),
		cameraCalibration.cameraMatrix, cameraCalibration.distCoeffs,
		rotationVector, translationVector);

	return cameraCalibration;
}

TransformationMatrix PoseFinder::findTransformaton(VectorOf3DPoints objectPoints, VectorOf2DPoints imagePoints, CameraCalibration cameraCalibration) {
	TransformationMatrix transformationMatrix;

	cv::solvePnPRansac(objectPoints, imagePoints, cameraCalibration.cameraMatrix,
		cameraCalibration.distCoeffs, transformationMatrix.rotationMatrix, transformationMatrix.translationMatrix);

	return transformationMatrix;
}

VectorOf2DPoints PoseFinder::getProjectedPoints(CameraCalibration cameraCalibration, TransformationMatrix transformationMatrix, VectorOf3DPoints objectPoints)
{
	VectorOf2DPoints projectedPoints;

	cv::projectPoints(objectPoints, transformationMatrix.rotationMatrix, transformationMatrix.translationMatrix,
		cameraCalibration.cameraMatrix, cameraCalibration.distCoeffs, projectedPoints);

	return projectedPoints;
}


std::vector<cv::Point3f> PoseFinder::getBottomOfTheCube3DPoints() {
	return std::vector<cv::Point3f>{ {0.0, 0.0, 0.0}, { 1.0, 0.0, 0.0 }, { 1.0, 1.0, 0.0 }, { 0.0, 1.0, 0.0 }};
}

std::vector<cv::Point3f> PoseFinder::getTopOfTheCube3DPoints() {
	return std::vector<cv::Point3f>{ { 0.0, 0.0, -1.0 }, { 1.0, 0.0, -1.0 }, { 1.0, 1.0, -1.0 }, { 0.0, 1.0, -1.0 }};
}
