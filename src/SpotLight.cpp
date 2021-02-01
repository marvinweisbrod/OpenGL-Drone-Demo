#include "SpotLight.h"

SpotLight::SpotLight(glm::vec3 position, glm::vec3 direction, float innerAngle, float outerAngle, glm::vec4 color, const glm::vec3& attenuation)
	: m_color(color)
	, m_dir(direction)
	, m_attenuation(attenuation)
	, m_angleInner(innerAngle)
	, m_angleOuter(outerAngle)
{
	setPosition(position);
}
