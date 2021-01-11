#include "Renderable.h"

Renderable::Renderable()
{
}

void Renderable::render(ShaderProgram& shader)
{
	if (m_meshes.empty()) return;
	shader.setUniform("modelMat", getTransformMatrix(), false);
	shader.setUniform("modelMatInvT", getInverseMatrix(), true);
	
	for (auto& mesh : m_meshes) {
		mesh->render(shader);
	}
}

void Renderable::addMesh(std::shared_ptr<Mesh>& mesh)
{
	m_meshes.push_back(mesh);
}
