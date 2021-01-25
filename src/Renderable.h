#pragma once

#include "CommonTypes.h"
#include "Mesh.h"
#include "Transform.h"
#include "ShaderProgram.h"
#include <libheaders.h>
#include <memory>

#include <vector>

class Renderable : public Transform {
public:
	Renderable();
	void render(ShaderProgram& shader);

	void addMesh(std::shared_ptr<Mesh>& mesh);

	void setBounds(const Bounds& b)
	{
		bounds = b;
	}

	const Bounds& getBounds() const
	{
		return bounds;
	}

	/// <summary>
	/// Get the bounds of this renderable in world space.
	/// </summary>
	/// <returns></returns>
	Bounds getTransformedBounds();
	void setActive(bool active = true);
	bool isActive() const;

private:
	std::vector<std::shared_ptr<Mesh>> m_meshes;
	Bounds bounds;
	bool active = true;
};

inline bool Renderable::isActive() const
{
	return active;
}