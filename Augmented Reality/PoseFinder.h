#pragma once
#include <glm/glm.hpp>
#include "opencv2/opencv.hpp"
#include <vector>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
//na bazie https://github.com/daviddoria/Examples/blob/master/c%2B%2B/OpenCV/SolvePNP/SolvePNP.cxx

#include "RegionBasedOperator.h"

using VectorOf3DPoints = std::vector<cv::Point3f>;
using VectorOf2DPoints = std::vector<cv::Point2f>;

struct CameraCalibration {
	cv::Mat cameraMatrix;
	cv::Mat distCoeffs;
};

struct TransformationMatrix {
	cv::Mat rotationMatrix;
	cv::Mat translationMatrix;
};

class PoseFinder 
	: public RegionBasedOperator
{
public:
	PoseFinder(const size_t borderSize, const size_t regionSize, const size_t stepSize);
	CameraCalibration calibrateCamera(std::vector<VectorOf3DPoints> objectPointsPatterns, std::vector<VectorOf2DPoints> imagePointsPatters);
	TransformationMatrix findTransformaton(VectorOf3DPoints objectPoints, VectorOf2DPoints imagePoints, CameraCalibration cameraCalibration);
	VectorOf2DPoints getProjectedPoints(CameraCalibration cameraCalibration, TransformationMatrix transformationMatrix, VectorOf3DPoints objectPoints);
	static std::vector<cv::Point3f> getBottomOfTheCube3DPoints();
	static std::vector<cv::Point3f> getTopOfTheCube3DPoints();
};

