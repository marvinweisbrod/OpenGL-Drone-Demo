#include "PointLight.h"


PointLight::PointLight(glm::vec3 position, glm::vec4 color, const glm::vec3& attenuation)
	: m_color(color), m_attenuation(attenuation)
{
	setPosition(position);
}
