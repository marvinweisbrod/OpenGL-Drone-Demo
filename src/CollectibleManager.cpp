#include "CollectibleManager.h"
#include "Renderable.h"

CollectibleManager::CollectibleManager(const std::shared_ptr<Renderable>& drone)
    : drone(drone)
{
}

void CollectibleManager::add(const std::shared_ptr<Renderable>& cake)
{
    Collectible collectible{ cake };
    collectibles.push_back(std::move(collectible));
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
