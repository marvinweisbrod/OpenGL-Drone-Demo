#include "Bounds.h"

Bounds::Bounds()
    : min(glm::vec3(0, 0, 0)), max(glm::vec3(0, 0, 0)), hasContent(false)
{
}

void Bounds::include(const glm::vec3& vertex)
{
    if(hasContent)
    {
        if(vertex.x < min.x)
        {
            min.x = vertex.x;
        }
        else if(vertex.x > max.x)
        {
            max.x = vertex.x;
        }

        if(vertex.y < min.y)
        {
            min.y = vertex.y;
        }
        else if(vertex.y > max.y)
        {
            max.y = vertex.y;
        }

        if(vertex.z < min.z)
        {
            min.z = vertex.z;
        }
        else if(vertex.z > max.z)
        {
            max.z = vertex.z;
        }
    }
    else
    {
        min = vertex;
        max = vertex;
        hasContent = true;
    }
}

void Bounds::include(const Bounds& other)
{
    if(other.hasContent)
    {
        if(hasContent)
        {
            if(other.min.x < min.x)
            {
                min.x = other.min.x;
            }
            else if(other.max.x > max.x)
            {
                max.x = other.max.x;
            }

            if(other.min.y < min.y)
            {
                min.y = other.min.y;
            }
            else if(other.max.y > max.y)
            {
                max.y = other.max.y;
            }

            if(other.min.z < min.z)
            {
                min.z = other.min.z;
            }
            else if(other.max.z > max.z)
            {
                max.z = other.max.z;
            }
        }
        else
        {
            min = other.min;
            max = other.max;
            hasContent = true;
        }
    }
}

bool Bounds::collide(const Bounds& other) const
{
    return glm::length(other.getCenter() - getCenter()) < (other.getRadius() + getRadius());
}

float Bounds::getRadius() const
{
    glm::vec3 center = getCenter();
    glm::vec3 diff = max - center;
    float radius = std::fmax(std::abs(diff.x), std::fmax(std::abs(diff.y), std::abs(diff.z)));
    return radius;
}

glm::vec3 Bounds::getCenter() const
{
    return (min + max) / 2.0f;
}
