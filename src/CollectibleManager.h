#pragma once

#include <vector>
#include <memory>
#include "CommonTypes.h"
#include "Transform.h"

class Renderable;
class TextRenderer;

class CollectibleManager
{
    struct Collectible
    {
        std::shared_ptr<Renderable> renderable;
        float time;
    };

public:
    CollectibleManager(const std::shared_ptr<Renderable>& drone, const std::shared_ptr<TextRenderer>& textRenderer);

    void add(const std::shared_ptr<Renderable>& cake);
    void update(float dt);
    void reset();
    int getUncollectedCollectibles() { return static_cast<int>(collectibles.size()); };
private:
    std::vector<Collectible>        collectibles;
    std::vector<Collectible>        collectedCollectibles;
    std::shared_ptr<Renderable>     drone;
    std::shared_ptr<TextRenderer>   textRenderer;
    size_t                          totalCollectibleCount = 0;
    unsigned                        collectionTextId = 0;
    unsigned                        winTextId = 0;
    unsigned                        resetTextId = 0;
};
