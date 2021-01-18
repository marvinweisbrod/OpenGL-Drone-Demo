#pragma once
#include "Transform.h"
#include <memory>
#include <libheaders.h>
#include "GameWindow.h"

class DroneController {
public:
	DroneController(std::shared_ptr<Transform>& theDrone, GameWindow* g);
	~DroneController();
	void update(float dt);
	struct State {
		glm::vec3 dirAccel; // directional acceleration
		glm::vec3 dirAccelMax;
		glm::vec3 rotAccel;
		glm::vec3 rotAccelMax;
		glm::vec3 rotSpeed;
		glm::vec3 rotSpeedMax;
		glm::vec3 dirSpeed;
		glm::vec3 dirSpeedMax;
	};
	const State& getState() { return state; };
private:
	std::shared_ptr<Transform> drone;
	GameWindow* gameWindow;
	State state;
	void checkInput(glm::vec3& out_dir, glm::vec3& out_rot);
	float flightControlAdjustment(float input, float speed, float accelerationMax);
	float custom_min(float a, float b) { return a < b ? a : b; };
};