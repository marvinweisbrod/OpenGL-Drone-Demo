#pragma once

#include "CommonTypes.h"
#include "Transform.h"
#include <libheaders.h>


class PointLight : public Transform {
public:
	PointLight(glm::vec3 position, glm::vec4 color);
	glm::vec4& getColor() { return m_color; };
private:
	glm::vec4 m_color;
};