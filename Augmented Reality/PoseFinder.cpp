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
	return std::vector<cv::Point3f>{ {0.0, 0.0, 0.0}, { 1.0, 0.0, 0.0 }, { 1.0, 1.0, 0.0 }, { 0.0, 1.0, 0.0 }};
}

std::vector<cv::Point3f> PoseFinder::getTopOfTheCube3DPoints() {
	return std::vector<cv::Point3f>{ { 0.0, 0.0, -1.0 }, { 1.0, 0.0, -1.0 }, { 1.0, 1.0, -1.0 }, { 0.0, 1.0, -1.0 }};
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

	viewMatrix *= invertYandZMatrix;

	std::cout << "Translation vector: " << translationVector << "\n";
	std::cout << "Rotation vector: " << rotationVector << "\n";
	std::cout << "Rotatin matrix: " << rotationMatrix << "\n";
	std::cout << "ViewMatrix: " << viewMatrix << "\n";

	glm::mat4 openGLViewMatrix;

	for (size_t row = 0; row < 4; row++) {
		std::cout << "[ ";
		for (size_t column = 0; column < 4; column++) {
			openGLViewMatrix[row][column] = viewMatrix.at<float>(row, column);
			std::cout << openGLViewMatrix[row][column] << " ";
		}
		std::cout << " ]\n";
	}

	return openGLViewMatrix;
}
