#pragma once
#include "Transform.h"
#include "DroneController.h"
#include <memory>
#include <libheaders.h>

class DroneAnimator {
public:
	DroneAnimator(std::shared_ptr<DroneController>& droneController, std::shared_ptr<Transform>& parent, std::shared_ptr<Transform>& rotor_FL, std::shared_ptr<Transform>& rotor_FR, std::shared_ptr<Transform>& rotor_BL, std::shared_ptr<Transform>& rotor_BR);
	~DroneAnimator();

	void update(float dt);
	struct State {
		float rotorSpeed;
		glm::vec3 anglePercentPerSecond;
		glm::vec3 currentState; // between [-1;+1]
		glm::vec3 anglesMax;

	};
private:
	std::shared_ptr<DroneController> controller;
	std::shared_ptr<Transform> parent;
	std::shared_ptr<Transform> rotorFL;
	std::shared_ptr<Transform> rotorFR;
	std::shared_ptr<Transform> rotorBL;
	std::shared_ptr<Transform> rotorBR;
	const DroneController::State& controllerState;
	State state;

	float calculateNewCurrentAngle(float current, float perSecond, float required, float dt);
	float custom_min(float a, float b) { return a < b ? a : b; };
};