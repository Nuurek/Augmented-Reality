#include "ViewMatrix.h"


glm::mat4 ViewMatrix::getViewMatrix() {
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

	viewMatrix = invertYandZMatrix * viewMatrix;

	cv::transpose(viewMatrix, viewMatrix);

	glm::mat4 openGLViewMatrix;

	for (size_t row = 0; row < 4; row++) {
		for (size_t column = 0; column < 4; column++) {
			openGLViewMatrix[row][column] = viewMatrix.at<float>(row, column);
		}
	}

	return openGLViewMatrix;
}