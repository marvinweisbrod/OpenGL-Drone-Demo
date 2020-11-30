#include "Renderable.h"

Renderable::Renderable()
{
}

void Renderable::render(ShaderProgram& shader)
{
	if (m_meshes.empty()) return;
	shader.setUniform("modelView", getTransformMatrix(), false);
	for (auto& mesh : m_meshes)
		mesh.render();
}

void Renderable::addMesh(Mesh& mesh)
{
	m_meshes.push_back(mesh);
}

void Renderable::addMesh_inplace(std::vector<Vertex>& vertices, std::vector<VertexAttribute>& vertexAttributes, std::vector<Index>& indices)
{
	m_meshes.emplace_back(vertices, vertexAttributes, indices);
}
