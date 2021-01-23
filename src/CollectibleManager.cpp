#include "CollectibleManager.h"
#include "Renderable.h"

CollectibleManager::CollectibleManager(const std::shared_ptr<Renderable>& drone)
    : drone(drone)
{
}

void CollectibleManager::add(const std::shared_ptr<Renderable>& cake)
{
    Collectible collectible{ cake, 0.0f };
    collectibles.push_back(std::move(collectible));
}

static float floorToMultiple(float value, float multiple)
{
    while(value > multiple)
    {
        value -= multiple;
    }

    return value;
}

void CollectibleManager::update(float dt)
{
    Bounds droneBounds = drone->getTransformedBounds();
    for(auto it = collectibles.begin(); it != collectibles.end(); )
    {
        if(!(*it).renderable->isActive())
        {
            ++it;
            continue;
        }

        // Update animation
        float frequency = 3.0f;
        float height = 0.5f;
        float oldYValue = height * std::max(std::sin((*it).time * frequency), 0.0f);
        float oldStartOfWave = floorToMultiple((*it).time, 2 * 3.14159265f / frequency);
        float oldSpin = oldStartOfWave < 3.14159265f / frequency ? (oldStartOfWave / (3.14159265f / frequency)) : 0.0f;
        (*it).time += dt;
        float newYValue = height * std::max(std::sin((*it).time * frequency), 0.0f);
        float newStartOfWave = floorToMultiple((*it).time, 2 * 3.14159265f / frequency);
        float newSpin = newStartOfWave < 3.14159265f / frequency ? (newStartOfWave / (3.14159265f / frequency)) : 0.0f;
        (*it).renderable->translate(glm::vec3(0.0f, newYValue - oldYValue, 0.0f));
        (*it).renderable->rotate(glm::angleAxis(glm::radians((newSpin - oldSpin) * 360.0f), glm::vec3(0.0f, 1.0f, 0.0f)));

        // Check for collision
        Bounds collectibleBounds = (*it).renderable->getTransformedBounds();
        if(collectibleBounds.collide(droneBounds))
        {
            (*it).renderable->setActive(false);
            it = collectibles.erase(it);
        }
        else
        {
            ++it;
        }
    }
}
