#pragma once
#include <glm/glm.hpp>
#include "opencv2/opencv.hpp"
#include <vector>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
//na bazie https://github.com/daviddoria/Examples/blob/master/c%2B%2B/OpenCV/SolvePNP/SolvePNP.cxx
class PoseFinder
{
public:
	static std::vector<cv::Point2f> projectedPoints;
	PoseFinder();
	~PoseFinder();
	static glm::mat4 findPose(std::vector<cv::Point2f> imagePoints, std::vector<cv::Point3f> objectPoints);
	static std::vector<cv::Point2f> getExample2DPoints();
	static std::vector<cv::Point3f> getExample3DPoints();
};

