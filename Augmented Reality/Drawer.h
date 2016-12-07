#pragma once
#include "ShaderProgram.h"
#include "Teapot.h"
#include "Model.h"
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
	void drawScene(cv::Mat frame,float angle);
	GLFWwindow* getWindow();
	
private:
	void drawObject(GLuint vao, ShaderProgram * shaderProgram, mat4 mP, mat4 mV, mat4 mM);
	void initOpenGLProgram(GLFWwindow * window);
	void assignVBOtoAttribute(ShaderProgram * shaderProgram, char * attributeName, GLuint bufVBO, int vertexSize);
	GLuint makeBuffer(void * data, int vertexCount, int vertexSize);
	GLuint readTexture(char* filename);
	GLuint readFrame(cv::Mat frame);
	void freeOpenGLProgram();
	GLuint tex0;
	GLuint currentFrameTex;

	ShaderProgram *shaderProgram;
	GLuint vao;
	GLuint bufVertices; //Uchwyt na bufor VBO przechowuj¹cy tablicê wspó³rzêdnych wierzcho³ków
	GLuint bufColors;  //Uchwyt na bufor VBO przechowuj¹cy tablicê kolorów
	GLuint bufNormals; //Uchwyt na bufor VBO przechowuj¹cy tablickê wektorów normalnych
	GLuint bufTexCoords;
	Models::Model model = Models::teapot;
	const float PI = 3.141592653589793f; 
	GLFWwindow* window;
	float aspectRatio = 4.f/3.f;
};

