#pragma once

#include "CommonTypes.h"
#include "Mesh.h"
#include <libheaders.h>
#include <vector>

class Renderable {
public:
	Renderable();
	void initialize();
	void render();

	void addMesh(Mesh& mesh);
	void addMesh_inplace(std::vector<Vertex>& vertices, std::vector<VertexAttribute>& vertexAttributes, std::vector<Index>& indices);
private:
	std::vector<Mesh> m_meshes;
	bool initialized = false;
};