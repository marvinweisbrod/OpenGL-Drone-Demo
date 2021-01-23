#pragma once

#include <vector>
#include <memory>
#include "CommonTypes.h"
#include "Transform.h"

class Renderable;

class CollectibleManager
{
    struct Collectible
    {
        std::shared_ptr<Renderable> renderable;
    };

public:
    CollectibleManager(const std::shared_ptr<Renderable>& drone);

    void add(const std::shared_ptr<Renderable>& cake);
    void update(float dt);

private:
    std::vector<Collectible>    collectibles;
    std::shared_ptr<Renderable> drone;
};
