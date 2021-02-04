#pragma once

#include "CommonTypes.h"
#include "Transform.h"
#include <libheaders.h>


class PointLight : public Transform {
public:
	PointLight(glm::vec3 position, glm::vec4 color, const glm::vec3& attenuation);
	glm::vec4& getColor() { return m_color; };
	glm::vec3& getAttenuation() { return m_attenuation; }
private:
	glm::vec4 m_color;
	glm::vec3 m_attenuation;
};