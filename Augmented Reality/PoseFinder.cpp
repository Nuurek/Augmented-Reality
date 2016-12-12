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

std::vector<cv::Point2f> PoseFinder::projectedPoints;
std::vector<cv::Point2f> PoseFinder::findPose(std::vector<cv::Point3f> objectPoints, std::vector<cv::Point2f> imagePoints)
{
	cv::Mat rotationVector;
	cv::Mat translationVector;
	
	cv::solvePnPRansac(objectPoints, imagePoints, cameraMatrix, distCoeffs, rotationVector, translationVector);

	std::vector<cv::Point3f> topOfCube3DPoints = getTopOfTheCube3DPoints();
	std::vector<cv::Point2f> topOfCubeProjectedPoints;

	cv::projectPoints(topOfCube3DPoints, rotationVector, translationVector, cameraMatrix, distCoeffs, topOfCubeProjectedPoints);

	return topOfCubeProjectedPoints;

	/*
	auto axis = std::vector<cv::Point3f>{ { 0.0, 0.0, 0.0 },{ 0.0, 1.0, 0.0 },{ 1.0, 1.0, 0.0 },{ 1.0, 0.0, 0.0 },
	{ 0.0, 0.0, -1.0 },{ 0.0, 1.0, -1.0 },{ 1.0, 1.0, -1.0 },{ 1.0, 0.0, -1.0 } };
	cv::projectPoints(axis, rvec, tvec, cameraMatrix, distCoeffs, projectedPoints);

	for (unsigned int i = 0; i < projectedPoints.size(); ++i)
	{
		std::cout << "Image point: " << imagePoints[i] << " Projected to " << projectedPoints[i] << "\n";

	}

	cv::Mat rotation, viewMatrix(4, 4, CV_64F);

	cv::Rodrigues(rvec, rotation);
	for (unsigned int row = 0; row<3; ++row)
	{
		for (unsigned int col = 0; col<3; ++col)
		{
			viewMatrix.at<double>(row, col) = rotation.at<double>(row, col);
		}
		viewMatrix.at<double>(row, 3) = tvec.at<double>(row, 0);
	}
	viewMatrix.at<double>(3, 3) = 1.0f;

	cv::Mat cvToGl = cv::Mat::zeros(4, 4, CV_64F); 
	cvToGl.at<double>(0, 0) = 1.0f; 
	cvToGl.at<double>(1, 1) = -1.0f;// Invert the y axis 
	cvToGl.at<double>(2, 2) = -1.0f; // invert the z axis 
	cvToGl.at<double>(3, 3) = 1.0f; 

	cv::Mat cvToGl2 = (cv::Mat_<double>(4, 4) << 1,1,1,1,   -1,-1,-1,-1,    -1,-1,-1,-1,     1,1,1,1);

	viewMatrix = cvToGl * viewMatrix;
	//or
	//viewMatrix = viewMatrix * cvToGl2;

	cv::Mat glViewMatrix = cv::Mat::zeros(4, 4, CV_64F);

	cv::transpose(viewMatrix, glViewMatrix);

	glm::mat4 result(1.0);
	for (int i = 0; i <= 3; i++)
		for (int j = 0; j <= 3; j++)
			result[i][j] = glViewMatrix.at<double>(i,j);
	std::cout<<"result " << glm::to_string(result) << "\n";
	
	return result;
	*/
}

void PoseFinder::calibrateCamera(std::vector<std::vector<cv::Point3f>> objectPointsPatterns, std::vector<std::vector<cv::Point2f>> imagePointsPatters) {
	cv::Mat rotationVector;
	cv::Mat translationVector;

	cv::calibrateCamera(objectPointsPatterns, imagePointsPatters, cv::Size(buffer->getWidth(), buffer->getHeight()), cameraMatrix, distCoeffs,
		rotationVector, translationVector);
}

std::vector<cv::Point3f> PoseFinder::getBottomOfTheCube3DPoints() {
	return std::vector<cv::Point3f>{ {0.0, 0.0, 0.0}, { 1.0, 0.0, 0.0 }, { 1.0, 1.0, 0.0 }, { 0.0, 1.0, 0.0 }};
}

std::vector<cv::Point3f> PoseFinder::getTopOfTheCube3DPoints() {
	return std::vector<cv::Point3f>{ { 0.0, 0.0, -1.0 }, { 1.0, 0.0, -1.0 }, { 1.0, 1.0, -1.0 }, { 0.0, 1.0, -1.0 }};
}
