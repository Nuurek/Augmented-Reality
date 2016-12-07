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

#define GLM_FORCE_RADIANS

using namespace glm;
class Drawer
{ 
public:
	Drawer();
	~Drawer();
	void init(int frameWidth, int frameHeight);
	void drawScene(cv::Mat *frame,float angle);
	GLFWwindow* getWindow();
	
private:
	void drawObject(GLuint vao, ShaderProgram * shaderProgram, mat4 mP, mat4 mV, mat4 mM, Models::Model object);
	void initOpenGLProgram(GLFWwindow * window);
	void assignVBOtoAttribute(ShaderProgram * shaderProgram, char * attributeName, GLuint bufVBO, int vertexSize);
	GLuint makeBuffer(void * data, int vertexCount, int vertexSize);
	GLuint readTexture(char* filename);
	GLuint initFrameTexture();
	void readFrame(cv::Mat *frame, GLuint tex);
	void freeOpenGLProgram();
	GLuint tex0;
	GLuint currentFrameTex;

	ShaderProgram *shaderProgram;
	ShaderProgram *backgroundShaderProgram;
	GLuint vao;
	GLuint backgroundVAO;
	Models::Model model = Models::teapot;
	Models::Model backgroundModel = Models::quad;
	const float PI = 3.141592653589793f; 
	GLFWwindow* window;
	float aspectRatio = 4.f/3.f;
};

