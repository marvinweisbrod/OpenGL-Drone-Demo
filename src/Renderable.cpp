#include "Renderable.h"

Renderable::Renderable()
{
}

void Renderable::render(ShaderProgram& shader)
{
	if(!active)
	{
		return;
	}

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
	bounds.include(mesh->getBounds());
}

Bounds Renderable::getTransformedBounds()
{
	glm::vec3 min = transformPosition(bounds.getMin());
	glm::vec3 max = transformPosition(bounds.getMax());

	Bounds bounds;
	bounds.include(min);
	bounds.include(max);
	bounds.include(glm::vec3(min.x, min.y, max.z));
	bounds.include(glm::vec3(min.x, max.y, min.z));
	bounds.include(glm::vec3(max.x, min.y, min.z));
	bounds.include(glm::vec3(min.x, max.y, max.z));
	bounds.include(glm::vec3(max.x, min.y, max.z));
	bounds.include(glm::vec3(max.x, max.y, min.z));

	return bounds;
}

void Renderable::setActive(bool a)
{
	active = a;
	for(Transform* child : getChildren())
	{
		static_cast<Renderable*>(child)->setActive(a);
	}
}
