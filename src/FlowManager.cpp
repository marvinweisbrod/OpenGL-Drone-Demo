#include "FlowManager.h"
#include <sstream>
#include <iomanip>

FlowManager::FlowManager(std::shared_ptr<DroneController> dC
	, std::shared_ptr<TextRenderer> tR
	, std::shared_ptr<CollectibleManager> cM
	, GameWindow* window)
	: droneController(dC)
	, textRenderer(tR)
	, collectibleManager(cM)
	, gameWindow(window)
{
	auto text = textRenderer->createTextEntry();
	textId = text.first;
	text.second->setPosition(glm::vec2(0.99f, 0.99f));
	text.second->setPositioning(glm::vec2(1.0f, 1.0f));
	text.second->setSize(0.1f);
	text.second->setText("Time: 00:00:00");

	startTime = std::chrono::high_resolution_clock::now();
}

void FlowManager::update()
{
	// resetting game if spacebar has been pressed this frame
	// we need resetWasPressedBefore to differentiate whether it is being held or has been pressed just now
	if (gameWindow->getInput().getKeyState(Key::Space) == KeyState::Pressed) {
		if (!resetWasPressedBefore) {
			droneController->reset();
			collectibleManager->reset();

			startTime = std::chrono::high_resolution_clock::now();
			endTime = startTime;
		}
		resetWasPressedBefore = true;
	}
	else
		resetWasPressedBefore = false;

	if (collectibleManager->getUncollectedCollectibles() != 0) {
		endTime = std::chrono::high_resolution_clock::now();
	}

	std::stringstream newText;
	newText << "Time: ";
	std::chrono::duration<float> elapsed = endTime - startTime;
	newText << std::setfill('0') << std::setw(7) << std::fixed  << std::setprecision(3) << elapsed.count();
	textRenderer->getTextEntryById(textId).setText(newText.str());
}
