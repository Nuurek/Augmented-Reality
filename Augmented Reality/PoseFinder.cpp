#include "PoseFinder.h"
#include <memory>
#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/calib3d/calib3d.hpp"
#include "opencv2/highgui/highgui.hpp"


PoseFinder::PoseFinder()
{
}


PoseFinder::~PoseFinder()
{
}

glm::mat4 PoseFinder::findPose(std::vector<cv::Point2f> imagePoints, std::vector<cv::Point3f> objectPoints)
{
	std::cout << "There are " << imagePoints.size() << " imagePoints and " << objectPoints.size() << " objectPoints." << std::endl;
	cv::Mat cameraMatrix(3, 3, cv::DataType<double>::type);
	cv::setIdentity(cameraMatrix);

	std::cout << "Initial cameraMatrix: " << cameraMatrix << std::endl;
	glm::mat4 P = glm::perspective(50 * 3.14f / 180, 4.f / 3.f, 1.0f, 50.0f);
	glm::mat4 V = glm::lookAt( //Wylicz macierz widoku
		glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(0.0f, 0.0f, 1.0f),
		glm::vec3(0.0f, -1.0f, 0.0f));
	glm::mat4 M = glm::mat4(1.0f);
	glm::mat4 PVM = P*V*M;
	std::cout << "GLM Projection:\n";
	for (auto point = objectPoints.begin(); point != objectPoints.end(); point++) {
		std::cout << "GLM 3d point: x" << *point;
		glm::vec3 GLMpoint3D((*point).x, (*point).y, (*point).z);
		glm::vec4 GLMpoint2D;
		GLMpoint2D = PVM * glm::vec4(GLMpoint3D,1.0f);
		std::cout << "GLM 2d point " << GLMpoint2D.x <<" " <<GLMpoint2D.y << " "<<GLMpoint2D.z <<" "<< GLMpoint2D.w <<"\n";
	}

	cv::Mat distCoeffs(4, 1, cv::DataType<double>::type);
	distCoeffs.at<double>(0) = 0;
	distCoeffs.at<double>(1) = 0;
	distCoeffs.at<double>(2) = 0;
	distCoeffs.at<double>(3) = 0;

	cv::Mat rvec(3, 1, CV_64F);
	cv::Mat tvec(3, 1, CV_64F);

	cv::solvePnP(objectPoints, imagePoints, cameraMatrix, distCoeffs, rvec, tvec, cv::SOLVEPNP_ITERATIVE);

	std::cout << "rvec: " << rvec << std::endl;
	std::cout << "tvec: " << tvec << std::endl;

	std::vector<cv::Point2f> projectedPoints;
	cv::projectPoints(objectPoints, rvec, tvec, cameraMatrix, distCoeffs, projectedPoints);

	for (unsigned int i = 0; i < projectedPoints.size(); ++i)
	{
		std::cout << "Image point: " << imagePoints[i] << " Projected to " << projectedPoints[i] << std::endl;

	}
	
	//glm::mat4 resultMatrix = glm::translate(glm::mat4(mR), vT / 1000.0f);  // convert to meters
		cv::Mat rotM;
	Rodrigues(rvec, rotM);
	cv::Mat rotTMat = rotM.t();
	glm::mat4 rvecGLM;
	float val = rotTMat.at<double>(0, 0);
	glm::mat4 mR = glm::mat4(1.0);
	mR[0].x = rotTMat.at<double>(0, 0);
	mR[0][1] = rotTMat.at<double>(0, 1);
	mR[0][2] = rotTMat.at<double>(0, 2);
	mR[1][0] = rotTMat.at<double>(1, 0);
	mR[1][1] = rotTMat.at<double>(1, 1);
	mR[1][2] = rotTMat.at<double>(1, 2);
	mR[2][0] = rotTMat.at<double>(2, 0);
	mR[2][1] = rotTMat.at<double>(2, 1);
	mR[2][2] = rotTMat.at<double>(2, 2);
	mR[0][3] = tvec.at<double>(0);
	mR[1][3] = tvec.at<double>(1);
	mR[2][3] = tvec.at<double>(2);

	glm::mat4 m2 = glm::mat4(1.0);
	m2[0][0] = rotM.ptr()[0];
	m2[0][1] = -rotM.ptr()[3];
	m2[0][2] = -rotM.ptr()[6];
	m2[0][3] = 0;
	m2[1][0] = rotM.ptr()[1];
	m2[1][1] = -rotM.ptr()[4];
	m2[1][2] = -rotM.ptr()[7];
	m2[1][3] = 0;
	m2[2][0] = rotM.ptr()[2];
	m2[2][1] = -rotM.ptr()[5];
	m2[2][2] = -rotM.ptr()[8];
	m2[2][3] = 0;
	m2[3][0] = tvec.ptr()[0];
	m2[3][1] = -tvec.ptr()[1];
	m2[3][2] = -tvec.ptr()[2];
	m2[3][3] = 1;

	int a = 20;
	a*= 50.f;
	a++;
	a /= 20;

	glm::vec3 tvecGLM;
	std::cout << val << "\n";
	tvecGLM.x = -tvec.data[0];
	tvecGLM.y = -tvec.data[1];
	tvecGLM.z = tvec.data[2];
	glm::mat4 resultMatrix = glm::mat4(1.0f);
	resultMatrix = glm::translate(resultMatrix, tvecGLM);
	m2 = glm::transpose(m2);

	double* _r = rotM.ptr<double>();
	double _pm[16] = { _r[0],_r[1],_r[2],tvec.ptr()[0],
		_r[3],_r[4],_r[5],tvec.ptr()[1],
		_r[6],_r[7],_r[8],tvec.ptr()[2],
			0.f,0.f,0.f,1.f};
	glm::mat4 newM = glm::make_mat4(_pm);
	PVM = P * mR * M;
	std::cout << "Wyniki z wyliczonej macierzy:\n";
	for (auto point = objectPoints.begin(); point != objectPoints.end(); point++) {
		std::cout << "GLM 3d point: x" << *point;
		glm::vec3 GLMpoint3D((*point).x, (*point).y, (*point).z);
		glm::vec4 GLMpoint2D;
		GLMpoint2D = PVM * glm::vec4(GLMpoint3D, 1.0f);
		std::cout << "GLM 2d point " << GLMpoint2D.x << " " << GLMpoint2D.y << " " << GLMpoint2D.z << " " << GLMpoint2D.w << "\n";
	}

	return mR;
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
	return points;

}

std::vector<cv::Point3f> PoseFinder::getExample3DPoints()
{
	std::vector<cv::Point3f> points;

	float x, y, z;

	x = .5; y = .5; z = -.5;
	points.push_back(cv::Point3f(x, y, z));

	x = .5; y = .5; z = .5;
	points.push_back(cv::Point3f(x, y, z));

	x = -.5; y = .5; z = .5;
	points.push_back(cv::Point3f(x, y, z));

	x = -.5; y = .5; z = -.5;
	points.push_back(cv::Point3f(x, y, z));

	return points;
}
