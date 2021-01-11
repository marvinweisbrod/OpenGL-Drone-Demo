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
private:
	std::vector<std::shared_ptr<Mesh>> m_meshes;
};