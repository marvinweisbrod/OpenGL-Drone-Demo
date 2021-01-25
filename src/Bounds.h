#pragma once

#include <CommonTypes.h>

/// <summary>
/// Describes the bounds of an object in local space.
/// </summary>
class Bounds
{
public:
    Bounds();

    /// <summary>
    /// Let the bounds include this position.
    /// </summary>
    /// <param name="vertex"></param>
    void include(const glm::vec3& vertex);
    /// <summary>
    /// Let the bounds include the other bounds.
    /// </summary>
    /// <param name="bounds"></param>
    void include(const Bounds& bounds);
    /// <summary>
    /// Checks whether this bounds collide with the other bounds.
    /// Note that they should be in the same coordinate system.
    /// </summary>
    /// <param name="other"></param>
    /// <returns></returns>
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
