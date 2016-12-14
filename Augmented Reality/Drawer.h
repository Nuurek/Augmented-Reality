#pragma once
#include "ShaderProgram.h"
#include "Teapot.h"
#include "Model.h"
#include "Quad.h"
#include "Cube.h"
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Model.h"
#include "opencv2/opencv.hpp"
#include "GLModel.h"
#include <vector>

#define GLM_FORCE_RADIANS

using namespace glm;
class Drawer
{ 
public:
	Drawer();
	~Drawer();
	void init(int frameWidth, int frameHeight);
	void drawScene(cv::Mat& frame, std::vector<glm::mat4> cameraMatrix, std::vector<int> objectId);
	GLFWwindow* getWindow();
private:
	void drawObject(GLuint vao, ShaderProgram * shaderProgram, mat4 mP, mat4 mV, mat4 mM, int vertexCount, GLuint texture);
	void drawBackground(GLuint vao, ShaderProgram * shader, Models::Model object, GLuint texture);
	void initOpenGLProgram(GLFWwindow * window);
	void assignVBOtoAttribute(ShaderProgram * shaderProgram, char * attributeName, GLuint bufVBO, int vertexSize);
	GLuint makeBuffer(void * data, int vertexCount, int vertexSize);
	GLuint initFrameTexture();
	void readFrame(cv::Mat& frame, GLuint tex);
	void freeOpenGLProgram();
	GLuint tex0;
	GLuint currentFrameTex;

	int frameWidth, frameHeight;

	ShaderProgram *shaderProgram;
	ShaderProgram *backgroundShaderProgram;
	GLuint vao;
	GLuint backgroundVAO;
	Models::Model model = Models::cube;
	Models::Model backgroundModel = Models::quad;
	const float PI = 3.141592653589793f; 
	GLFWwindow* window;
	float aspectRatio = 4.f/3.f;
	std::vector<GLModel*> glModels;
};

