#pragma once

#include "CommonTypes.h"
#include <libheaders.h>
#include <vector>

class Mesh {
public:
	Mesh(std::vector<Vertex>& vertices, std::vector<VertexAttribute>& vertexAttributes, std::vector<Index>& indices);
	~Mesh();
	void initialize();
	void render();
	std::vector<Vertex>& getVertices() { return m_vertices; };
	std::vector<VertexAttribute>& getVertexAttributes() { return m_vertexAttributes; };
	std::vector<Index>& getIndices() { return m_indices; };
private:
	std::vector<Vertex> m_vertices;
	std::vector<VertexAttribute> m_vertexAttributes;
	std::vector<Index> m_indices;
	GLuint vaoID = 0;
	GLuint vboID = 0; 
	GLuint eboID = 0;
	bool initialized = false;
};