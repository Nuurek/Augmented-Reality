#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "opencv2/opencv.hpp"


struct ViewMatrix {
	cv::Mat rotationVector;
	cv::Mat translationVector;

	glm::mat4 getViewMatrix();
};
