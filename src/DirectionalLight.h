#pragma once

#include "Transform.h"

class DirectionalLight : public Transform
{
public:
    DirectionalLight(const glm::vec3& direction, const glm::vec4& color);

    glm::vec4& getColor() { return m_color; }
    void setDirection(const glm::vec3& direction);

private:
    glm::vec4 m_color;
};
