#pragma once

#include <GL/glew.h>
class GLModel
{
public:
	GLuint texture;
	int vertexCount;
	GLuint vao;
	GLModel();
	~GLModel();
};

