#include "Renderable.h"

Renderable::Renderable()
{
}

void Renderable::initialize()
{
	for (auto& mesh : m_meshes)
		mesh.initialize();
}

void Renderable::render()
{
	for (auto& mesh : m_meshes)
		mesh.render();
}

void Renderable::addMesh(Mesh& mesh)
{
	m_meshes.push_back(mesh);
	initialized = false;
}

void Renderable::addMesh_inplace(std::vector<Vertex>& vertices, std::vector<VertexAttribute>& vertexAttributes, std::vector<Index>& indices)
{
	m_meshes.emplace_back(vertices, vertexAttributes, indices);
	initialized = false;
}
