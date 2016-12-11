#include "PoseFinder.h"
#include <memory>
#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/calib3d/calib3d.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "glm/gtx/string_cast.hpp"

PoseFinder::PoseFinder()
{
}


PoseFinder::~PoseFinder()
{
}
std::vector<cv::Point2f> PoseFinder::projectedPoints;
glm::mat4 PoseFinder::findPose(std::vector<cv::Point2f> imagePoints, std::vector<cv::Point3f> objectPoints)
{
	projectedPoints.clear();

	std::cout << "There are " << imagePoints.size() << " imagePoints and " << objectPoints.size() << " objectPoints." << std::endl;
	cv::Mat cameraMatrix(3, 3, cv::DataType<double>::type);
	cv::setIdentity(cameraMatrix);

	std::cout << "Initial cameraMatrix: " << cameraMatrix << std::endl;

	cv::Mat distCoeffs(4, 1, cv::DataType<double>::type);
	distCoeffs.at<double>(0) = 0;
	distCoeffs.at<double>(1) = 0;
	distCoeffs.at<double>(2) = 0;
	distCoeffs.at<double>(3) = 0;

	cv::Mat rvec(3, 1, CV_64F);
	cv::Mat tvec(3, 1, CV_64F);

	cv::solvePnP(objectPoints, imagePoints, cameraMatrix, distCoeffs, rvec, tvec);

	std::cout << "rvec: " << rvec << std::endl;
	std::cout << "tvec: " << tvec << std::endl;

	cv::projectPoints(objectPoints, rvec, tvec, cameraMatrix, distCoeffs, projectedPoints);

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
}

std::vector<cv::Point2f> PoseFinder::getExample2DPoints()
{
	std::vector<cv::Point2f> points;

	float x, y;
	x = 314; y = 231;
	points.push_back(cv::Point2f(x, y));

	x = 461; y = 231;
	points.push_back(cv::Point2f(x, y));

	x = 457; y = 368;
	points.push_back(cv::Point2f(x, y));

	x = 299; y = 367;
	points.push_back(cv::Point2f(x, y));
	return points;

}

std::vector<cv::Point3f> PoseFinder::getExample3DPoints()
{
	std::vector<cv::Point3f> points;

	float x, y, z;

	x = -1.; y = 1.; z = -1.;
	points.push_back(cv::Point3f(x, y, z));

	x = 1.; y = 1.; z = -1.;
	points.push_back(cv::Point3f(x, y, z));

	x = 1.; y = -1.; z = -1.;
	points.push_back(cv::Point3f(x, y, z));

	x = -1.; y = -1.; z = -1.;
	points.push_back(cv::Point3f(x, y, z));

	return points;
}
