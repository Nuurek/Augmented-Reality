#include "Quad.h"
namespace Models {

	Quad quad;

	Quad::Quad() {
		vertices = QuadInternal::vertices;
		normals = QuadInternal::normals;
		vertexNormals = QuadInternal::vertexNormals;
		texCoords = QuadInternal::texCoords;
		colors = QuadInternal::colors;
		vertexCount = QuadInternal::vertexCount;
	}

	Quad::~Quad() {
	}

	namespace QuadInternal {
		unsigned int vertexCount = 6;

		float vertices[] = {
			-1.0f, -1.0f, 1.0f, 1.0f,
			1.0f, -1.0f, 1.0f, 1.0f,
			-1.0f, 1.0f, 1.0f, 1.0f,
			1.0f, -1.0f, 1.0f, 1.0f,
			-1.0f, 1.0f, 1.0f, 1.0f,
			1.0f,1.0f, 1.0f, 1.0f,
		};
		float texCoords[] = {
			0.0f, 1.0f,
			1.0f, 1.0f,
			0.0f,  0.0f,
			1.0f, 1.0f,
			0.0f,  0.0f,
			1.0f,  0.0f
		};


		float normals[] = {
			0.0f, 1.0f, 0.0, 0.0f,
			0.0f, 1.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f, 0.0f,

			0.0f, 1.0f, 0.0, 0.0f,
			0.0f, 1.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f, 0.0f
		};
		float colors[] = {
			1.0f,0.0f,0.0f,1.0f,
			1.0f,0.0f,0.0f,1.0f,
			1.0f,0.0f,0.0f,1.0f,

			1.0f,0.0f,0.0f,1.0f,
			1.0f,0.0f,0.0f,1.0f,
			1.0f,0.0f,0.0f,1.0f
		};

		float vertexNormals[] = {
			0.0f, 1.0f, 0.0, 0.0f,
			0.0f, 1.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f, 0.0f,

			0.0f, 1.0f, 0.0, 0.0f,
			0.0f, 1.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f, 0.0f
		};
	}
}