#include "CollectibleManager.h"
#include "Renderable.h"
#include "TextRenderer.h"
#include <algorithm>

CollectibleManager::CollectibleManager(const std::shared_ptr<Renderable>& drone, const std::shared_ptr<TextRenderer>& textRenderer)
    : drone(drone), textRenderer(textRenderer)
{
    auto entry = textRenderer->createTextEntry();
    collectionTextId = entry.first;
    entry.second->setPosition(glm::vec2(-0.99f, 0.99f));
    entry.second->setPositioning(glm::vec2(-1.0f, 1.0f));
    entry.second->setSize(0.1f);

    entry = textRenderer->createTextEntry();
    winTextId = entry.first;
    entry.second->setText("YOU WON!");
    entry.second->setPosition(glm::vec2(0.0f, 0.0f));
    entry.second->setSize(0.3f);
    entry.second->setPositioning(glm::vec2(0.0f,-1.0f));
    entry.second->setEnabled(false);

    entry = textRenderer->createTextEntry();
    resetTextId = entry.first;
    entry.second->setText("(press 'SPACE' to reset)");
    entry.second->setPosition(glm::vec2(0.0f, 0.0f));
    entry.second->setSize(0.05f);
    entry.second->setPositioning(glm::vec2(0.0f, 1.0f));
    entry.second->setEnabled(false);
}

void CollectibleManager::add(const std::shared_ptr<Renderable>& cake)
{
    Collectible collectible{ cake, 0.0f };
    collectibles.push_back(std::move(collectible));
    ++totalCollectibleCount;
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
        // The y value at the old time. Use a sin wave, and only translate while the y value is above 0.
        float oldYValue = height * std::max(std::sin((*it).time * frequency), 0.0f);
        // Calculate the old rotation values. Rotate while the sin wave is above 0, so normalize the x values to 0..1 in that hill of the wave.
        float oldStartOfWave = floorToMultiple((*it).time, 2 * 3.14159265f / frequency);
        float oldSpin = oldStartOfWave < 3.14159265f / frequency ? (oldStartOfWave / (3.14159265f / frequency)) : 0.0f;
        (*it).time += dt;
        // The y value at the new time.
        float newYValue = height * std::max(std::sin((*it).time * frequency), 0.0f);
        // Calculate the new rotation values.
        float newStartOfWave = floorToMultiple((*it).time, 2 * 3.14159265f / frequency);
        float newSpin = newStartOfWave < 3.14159265f / frequency ? (newStartOfWave / (3.14159265f / frequency)) : 0.0f;
        // Calculate the difference of the rotation values (with error checking).
        float spin = newSpin >= oldSpin ? newSpin - oldSpin : (oldSpin < 1.0f) ? 1.0f - oldSpin : 0.0f;
        // Translate about the difference of the y values.
        (*it).renderable->translate(glm::vec3(0.0f, newYValue - oldYValue, 0.0f));
        // Rotate by the difference of the rotation values.
        (*it).renderable->rotate(glm::angleAxis(glm::radians(spin * 180.0f), glm::vec3(0.0f, 1.0f, 0.0f)));

        // Check for collision
        Bounds collectibleBounds = (*it).renderable->getTransformedBounds();
        if(collectibleBounds.collide(droneBounds))
        {
            (*it).renderable->setActive(false);
            collectedCollectibles.push_back(*it);
            it = collectibles.erase(it);
        }
        else
        {
            ++it;
        }
    }

    // Update the text, stating how many collectibles already have been collected.
    std::stringstream buffer;
    buffer << "Collectibles: " << totalCollectibleCount - collectibles.size() << " / " << totalCollectibleCount;
    textRenderer->getTextEntryById(collectionTextId).setText(buffer.str());

    textRenderer->getTextEntryById(winTextId).setEnabled(collectibles.size() == 0 ? true : false);
    textRenderer->getTextEntryById(resetTextId).setEnabled(collectibles.size() == 0 ? true : false);
}

void CollectibleManager::reset()
{
    for (auto& collectible : collectedCollectibles) {
        collectible.renderable->setActive(true);
        collectibles.push_back(collectible);
    }
    collectedCollectibles.clear();
}
