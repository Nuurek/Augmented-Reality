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
void PoseFinder::init() {

	cameraMatrix = (cv::Mat_<double>(3, 3) << 6.5746697944293521e+002, 0., 3.1950000000000000e+002, 0.,
		6.5746697944293521e+002, 2.3950000000000000e+002, 0., 0., 1.);
	distCoeffs = (cv::Mat_<double>(4,1) << -4.1802327176423804e-001, 5.0715244063187526e-001, 0., 0.,
		-5.7843597214487474e-001);
}

PoseFinder::~PoseFinder()
{
}
std::vector<cv::Point2f> PoseFinder::projectedPoints;
std::vector<cv::Point2f> PoseFinder::axeis2DPoints;
cv::Mat PoseFinder::cameraMatrix;
cv::Mat PoseFinder::distCoeffs;
glm::mat4 PoseFinder::findPose(std::vector<cv::Point2f> imagePoints, std::vector<cv::Point3f> objectPoints)
{
	projectedPoints.clear();
	objectPoints = getExample3DPoints();

	for (unsigned int i = 0; i < imagePoints.size(); ++i)
	{
		std::cout << imagePoints[i] << std::endl;
	}

	for (unsigned int i = 0; i < objectPoints.size(); ++i)
	{
		std::cout << objectPoints[i] << std::endl;
	}
	std::cout << "There are " << imagePoints.size() << " imagePoints and " << objectPoints.size() << " objectPoints." << std::endl;
	//cv::Mat cameraMatrix(3, 3, cv::DataType<double>::type);
	//cv::setIdentity(cameraMatrix);

	std::cout << "Initial cameraMatrix: " << cameraMatrix << std::endl;
	//cv::Mat distCoeffs(4, 1, cv::DataType<double>::type);

	//distCoeffs.at<double>(0) = 0;
	//distCoeffs.at<double>(1) = 0;
	//distCoeffs.at<double>(2) = 0;
	//distCoeffs.at<double>(3) = 0;
	cv::Mat rvec(3, 1, CV_64F);
	cv::Mat tvec(3, 1, CV_64F);

	bool useExtrinsicGuess = false;
	int iterationsCount = 500;    // number of Ransac iterations.
	int reprojectionError = 2.0;  // maximum allowed distance to consider it an inlier.
	float confidence = 0.95;      // ransac successful confidence.

	//cv::solvePnPRansac(objectPoints, imagePoints, cameraMatrix, distCoeffs, rvec, tvec, useExtrinsicGuess, iterationsCount, reprojectionError, confidence);
	//cv::solvePnPRansac(objectPoints, imagePoints, cameraMatrixLocal, distCoeffsLocal, rvec, tvec);
	cv::solvePnP(objectPoints, imagePoints, cameraMatrix, distCoeffs, rvec, tvec);

	std::cout << "rvec: " << rvec << std::endl;
	std::cout << "tvec: " << tvec << std::endl;

	float length = 1;
	float x, y, z;
	std::vector< cv::Point3f > axisPoints;

	x = -length; y = -length; z = -length;
	axisPoints.push_back(cv::Point3f(x, y, z));

	x = -length; y = length; z = -length;
	axisPoints.push_back(cv::Point3f(x, y, z));

	x = -length; y = -length; z = -2*length;
	axisPoints.push_back(cv::Point3f(x, y, z));

	x = -length; y = -length; z = length;
	axisPoints.push_back(cv::Point3f(x, y, z));


	x = length; y = -length; z = -length;
	axisPoints.push_back(cv::Point3f(x, y, z));

	static std::vector<cv::Point2f> axeis2D;

	cv::projectPoints(axisPoints, rvec, tvec, cameraMatrix, distCoeffs, axeis2D);
	cv::projectPoints(objectPoints, rvec, tvec, cameraMatrix, distCoeffs, projectedPoints);


	axeis2DPoints.insert(axeis2DPoints.end(), axeis2D.begin(), axeis2D.end());

	for (unsigned int i = 0; i < projectedPoints.size(); ++i)
	{
		std::cout << "Image point: " << imagePoints[i] << " Projected to " << projectedPoints[i] << "\n";

	}

	cv::Mat rotation, viewMatrix= cv::Mat::zeros(4, 4, CV_64F);


	cv::Rodrigues(rvec, rotation);


	//for (unsigned int row = 0; row<3; ++row)
	//{
	//	for (unsigned int col = 0; col<3; ++col)
	//	{
	//		viewMatrix.at<double>(row, col) = rotation.at<double>(row, col);
	//	}
	//	viewMatrix.at<double>(row, 3) = tvec.at<double>(row, 0);
	//}
	viewMatrix.at<double>(0, 0) = rotation.at<double>(0, 0);
	viewMatrix.at<double>(0, 1) = rotation.at<double>(0, 1);
	viewMatrix.at<double>(0, 2) = rotation.at<double>(0, 2);
	viewMatrix.at<double>(0, 3) = tvec.at<double>(0);

	viewMatrix.at<double>(1, 0) = rotation.at<double>(1, 0);
	viewMatrix.at<double>(1, 1) = rotation.at<double>(1, 1);
	viewMatrix.at<double>(1, 2) = rotation.at<double>(1, 2);
	viewMatrix.at<double>(1, 3) = tvec.at<double>(1);

	viewMatrix.at<double>(2, 0) = rotation.at<double>(2, 0);
	viewMatrix.at<double>(2, 1) = rotation.at<double>(2, 1);
	viewMatrix.at<double>(2, 2) = rotation.at<double>(2, 2);
	viewMatrix.at<double>(2, 3) = tvec.at<double>(2);

	viewMatrix.at<double>(3, 0) = 0.0f;
	viewMatrix.at<double>(3, 1) = 0.0f;
	viewMatrix.at<double>(3, 2) = 0.0f;
	viewMatrix.at<double>(3, 3) = 1.0f;

	//for (unsigned int row = 0; row<3; ++row)
	//{
	//	for (unsigned int col = 0; col<3; ++col)
	//	{
	//		viewMatrix.at<double>(row, col) = rotation.at<double>(row, col);
	//	}
	//	viewMatrix.at<double>(row, 3) = tvec.at<double>(row, 0);
	//}
	//viewMatrix.at<double>(3, 3) = 1.0f;

	std::cout << "viewMatrix " << viewMatrix << "\n";

	std::cout << "view Mat :" << viewMatrix << "\n";
	cv::Mat cvToGl = cv::Mat::zeros(4, 4, CV_64F); 

	cvToGl.at<double>(0, 0) = 1.0f; 
	cvToGl.at<double>(1, 1) = -1.0f;// Invert the y axis 
	cvToGl.at<double>(2, 2) = -1.0f; // invert the z axis 
	cvToGl.at<double>(3, 3) = 1.0f; 

	cv::Mat cvToGl2 = (cv::Mat_<double>(4, 4) << 1,1,1,1,   -1,-1,-1,-1,    -1,-1,-1,-1,     1,1,1,1);

	viewMatrix = cvToGl * viewMatrix ;
	//or
	//viewMatrix = viewMatrix * cvToGl2;

	cv::Mat glViewMatrix = cv::Mat::zeros(4, 4, CV_64F);

	cv::transpose(viewMatrix, viewMatrix);

	glm::mat4 result(1.0);
	for (int i = 0; i <= 3; i++)
		for (int j = 0; j <= 3; j++)
			result[i][j] = viewMatrix.at<double>(i,j);

	//result = glm::transpose(result);
	std::cout << "result1 " << glm::to_string(result) << "\n";



	glm::mat4 result2(1.0);

	result2 = glm::rotate(result2, (float)rvec.at<double>(0), glm::vec3(1, 0, 0));
	result2 = glm::rotate(result2, -(float)rvec.at<double>(1), glm::vec3(0, 1, 0));
	result2 = glm::rotate(result2, -(float)rvec.at<double>(2), glm::vec3(0, 0, 1));
	result2 = glm::translate(result2, glm::vec3(tvec.at<double>(0), -tvec.at<double>(1), -tvec.at<double>(2)));


	std::cout<<"result2 " << glm::to_string(result2) << "\n";


	cv::Mat R;
	cv::Rodrigues(rvec, R); // R is 3x3
	R = R.t();  // rotation of inverse
	tvec = -R * tvec; // translation of inverse

	cv::Mat T(4, 4, R.type()); // T is 4x4
	T(cv::Range(0, 3), cv::Range(0, 3)) = R * 1; // copies R into T
	T(cv::Range(0, 3), cv::Range(3, 4)) = tvec * 1; // copies tvec into T
													// fill the last row of T (NOTE: depending on your types, use float or double)
	double *p = T.ptr<double>(3);
	p[0] = p[1] = p[2] = 0; p[3] = 1;

	cv::Mat RotX = (cv::Mat_<double>(4,4) << 1,0,0,0,
										0,-1,0,0,
										0,0,-1,0,
										0,0,0,1); // 4x4 matrix with a 180 deg rotation around X
	cv::Mat Tgl = T * RotX; // OpenGL camera in the object frame


	std::cout << " TGL: " << Tgl << "\n";
	glm::mat4 result3(1.0);
	for (int i = 0; i <= 3; i++)
		for (int j = 0; j <= 3; j++)
			result3[i][j] = Tgl.at<double>(i, j);

	result3 = glm::transpose(result3);
	return result;
}

std::vector<cv::Point2f> PoseFinder::getExample2DPoints()
{
	std::vector<cv::Point2f> points;

	float x, y;
	x = 282; y = 274;
	points.push_back(cv::Point2f(x, y));

	x = 397; y = 227;
	points.push_back(cv::Point2f(x, y));

	x = 577; y = 271;
	points.push_back(cv::Point2f(x, y));

	x = 462; y = 318;
	points.push_back(cv::Point2f(x, y));

	x = 270; y = 479;
	points.push_back(cv::Point2f(x, y));

	x = 450; y = 523;
	points.push_back(cv::Point2f(x, y));

	x = 566; y = 475;
	points.push_back(cv::Point2f(x, y));
	return points;

}

std::vector<cv::Point3f> PoseFinder::getExample3DPoints()
{
	std::vector<cv::Point3f> points;

	float x, y, z;


	x = 1; y = 1.; z = -1;
	points.push_back(cv::Point3f(x, y, z));

	x = 1; y = -1; z = -1;
	points.push_back(cv::Point3f(x, y, z));

	x = -1; y = -1.; z = -1;
	points.push_back(cv::Point3f(x, y, z));

	x = -1; y = 1.; z = -1;
	points.push_back(cv::Point3f(x, y, z));

	return points;
}
