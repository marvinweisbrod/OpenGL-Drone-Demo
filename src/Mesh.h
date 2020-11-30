#pragma once

#include "CommonTypes.h"
#include <libheaders.h>
#include <vector>

class Mesh {
public:
	Mesh(std::vector<Vertex>& vertices, std::vector<VertexAttribute>& vertexAttributes, std::vector<Index>& indices);
	~Mesh();
	void render();
private:
	GLuint vaoID = 0;
	GLuint vboID = 0; 
	GLuint iboID = 0;
	GLuint indexCount = 0;
};