#include "PointLight.h"


PointLight::PointLight(glm::vec3 position, glm::vec4 color)
	: m_color(color)
{
	setPosition(position);
}
