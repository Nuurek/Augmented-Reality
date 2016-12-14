#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "opencv2/opencv.hpp"
#include <vector>

#include "RegionBasedOperator.h"
#include "CameraCalibration.h"
#include "ViewMatrix.h"

using VectorOf3DPoints = std::vector<cv::Point3f>;
using VectorOf2DPoints = std::vector<cv::Point2f>;


class PoseFinder 
{
	CameraCalibration averageCameraCalibration;
	const unsigned long maxNumberOfCalibrations = 1000;
	unsigned long numberOfCalibrations = 0;

public:
	ViewMatrix findTransformaton(VectorOf3DPoints objectPoints, VectorOf2DPoints imagePoints, CameraCalibration cameraCalibration);
	VectorOf2DPoints getProjectedPoints(CameraCalibration cameraCalibration, ViewMatrix transformationVector, VectorOf3DPoints objectPoints);
	static std::vector<cv::Point3f> getBottomOfTheCube3DPoints(int orientation);
	static std::vector<cv::Point3f> getTopOfTheCube3DPoints();
};

