#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "opencv2/opencv.hpp"
#include <vector>

#include "RegionBasedOperator.h"

using VectorOf3DPoints = std::vector<cv::Point3f>;
using VectorOf2DPoints = std::vector<cv::Point2f>;

struct CameraCalibration {
	cv::Mat cameraMatrix;
	cv::Mat distCoeffs;

	CameraCalibration(float width, float height);
	CameraCalibration();
};

struct TransformationMatrix {
	cv::Mat rotationVector;
	cv::Mat translationVector;

	glm::mat4 getViewMatrix();
};

class PoseFinder 
	: public RegionBasedOperator
{
	CameraCalibration averageCameraCalibration;
	const unsigned long maxNumberOfCalibrations = 1000;
	unsigned long numberOfCalibrations = 0;

public:
	PoseFinder(const size_t borderSize, const size_t regionSize, const size_t stepSize);
	CameraCalibration calibrateCamera(std::vector<VectorOf3DPoints> objectPointsPatterns, std::vector<VectorOf2DPoints> imagePointsPatters);
	TransformationMatrix findTransformaton(VectorOf3DPoints objectPoints, VectorOf2DPoints imagePoints, CameraCalibration cameraCalibration);
	VectorOf2DPoints getProjectedPoints(CameraCalibration cameraCalibration, TransformationMatrix transformationVector, VectorOf3DPoints objectPoints);
	static std::vector<cv::Point3f> getBottomOfTheCube3DPoints();
	static std::vector<cv::Point3f> getTopOfTheCube3DPoints();
};

