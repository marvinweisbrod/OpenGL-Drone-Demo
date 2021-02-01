#include "DirectionalLight.h"

DirectionalLight::DirectionalLight(const glm::vec3& direction, const glm::vec4& color)
    : m_color(color)
{
    lookinto(direction);
}
