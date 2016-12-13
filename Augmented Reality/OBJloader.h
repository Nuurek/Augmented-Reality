#pragma once
#include <vector>
#include <glm/glm.hpp>
#include "Drawer.h"
#include "GLModel.h"
class OBJloader
{
public:
	bool loadOBJ(const char * path);
	GLModel* getGlModel(ShaderProgram* shaderProgram, char* textureName);
private:
	std::vector< unsigned int > vertexIndices, uvIndices, normalIndices;
	std::vector< glm::vec3 > temp_vertices;
	std::vector< glm::vec2 > temp_uvs;
	std::vector< glm::vec3 > temp_normals;

	std::vector<glm::vec4> out_vertices;
	std::vector<glm::vec2> out_uvs;
	std::vector<glm::vec4> out_normals;

	GLuint makeBuffer(void * data, int vertexCount, int vertexSize);
	void assignVBOtoAttribute(ShaderProgram * shaderProgram, char * attributeName, GLuint bufVBO, int vertexSize);
	GLuint readTexture(char * filename);
	
};

