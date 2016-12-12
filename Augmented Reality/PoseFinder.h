#pragma once
#include <glm/glm.hpp>
#include "opencv2/opencv.hpp"
#include <vector>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
//na bazie https://github.com/daviddoria/Examples/blob/master/c%2B%2B/OpenCV/SolvePNP/SolvePNP.cxx

#include "RegionBasedOperator.h"

class PoseFinder 
	: public RegionBasedOperator
{
	cv::Mat cameraMatrix;
	cv::Mat distCoeffs;

public:
	static std::vector<cv::Point2f> projectedPoints;
	PoseFinder(const size_t borderSize, const size_t regionSize, const size_t stepSize);
	std::vector<cv::Point2f> findPose(std::vector<cv::Point3f> objectPoints, std::vector<cv::Point2f> imagePoints);
	void calibrateCamera(std::vector<std::vector<cv::Point3f>> objectPointsPatterns, std::vector<std::vector<cv::Point2f>> imagePointsPatters);
	static std::vector<cv::Point3f> getBottomOfTheCube3DPoints();
	static std::vector<cv::Point3f> getTopOfTheCube3DPoints();
};

