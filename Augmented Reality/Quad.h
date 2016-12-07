#pragma once

#include "model.h"

namespace Models {
	namespace QuadInternal {
		extern float vertices[];
		extern float normals[];
		extern float vertexNormals[];
		extern float texCoords[];
		extern float colors[];
		extern unsigned int vertexCount;
	}

	class Quad : public Model {
	public:
		Quad();
		virtual ~Quad();
	};

	extern Quad quad;
}