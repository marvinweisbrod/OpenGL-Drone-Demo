#pragma once

#include <CommonTypes.h>

class Bounds
{
public:
    Bounds();

    void include(const glm::vec3& vertex);
    void include(const Bounds& bounds);
    bool collide(const Bounds& other) const;

    const glm::vec3& getMin() const
    {
        return min;
    }

    const glm::vec3& getMax() const
    {
        return max;
    }

    float getRadius() const;
    glm::vec3 getCenter() const;
    
protected:
    glm::vec3   min;
    glm::vec3   max;
    bool        hasContent;
};
