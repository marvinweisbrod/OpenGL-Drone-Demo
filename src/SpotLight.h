#pragma once

#include "CommonTypes.h"
#include "Transform.h"
#include <libheaders.h>


class SpotLight : public Transform {
public:
	SpotLight(glm::vec3 position, glm::vec3 direction, float innerAngle, float outerAngle, glm::vec4 color, const glm::vec3& attenuation);
	glm::vec4& getColor() { return m_color; };
	glm::vec3& getDirection() { return m_dir; };
	glm::vec2 getAnglesInnerOuter() { return glm::vec2(m_angleInner, m_angleOuter); };
	glm::vec3& getAttenuation() { return m_attenuation; }
private:
	glm::vec4 m_color;
	glm::vec3 m_dir;
	glm::vec3 m_attenuation;
	float m_angleInner;
	float m_angleOuter;
};