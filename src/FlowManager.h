#pragma once
#include "TextRenderer.h"
#include "CollectibleManager.h"
#include "Renderable.h"
#include <memory>
#include <libheaders.h>
#include "GameWindow.h"
#include "DroneController.h"
#include <chrono>

class FlowManager {
public:
	FlowManager(std::shared_ptr<DroneController> dC
		, std::shared_ptr<TextRenderer> tR
		, std::shared_ptr<CollectibleManager> cM
		, GameWindow* window);
	void update();
private:
	std::shared_ptr<DroneController>	droneController;
	std::shared_ptr<TextRenderer>		textRenderer;
	std::shared_ptr<CollectibleManager>	collectibleManager;
	GameWindow* gameWindow;
	unsigned textId;

	bool resetWasPressedBefore = false;
	std::chrono::steady_clock::time_point startTime;
	std::chrono::steady_clock::time_point endTime;
};