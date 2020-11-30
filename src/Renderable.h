#pragma once

#include "CommonTypes.h"
#include "Mesh.h"
#include "Transform.h"
#include "ShaderProgram.h"
#include <libheaders.h>

#include <vector>

class Renderable : public Transform {
public:
	Renderable();
	void render(ShaderProgram& shader);

	void addMesh(Mesh& mesh);
	void addMesh_inplace(std::vector<Vertex>& vertices, std::vector<VertexAttribute>& vertexAttributes, std::vector<Index>& indices);
private:
	std::vector<Mesh> m_meshes;
};