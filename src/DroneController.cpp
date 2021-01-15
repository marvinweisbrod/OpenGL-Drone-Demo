#include "DroneController.h"
#include <iostream>
#include <cmath>

DroneController::DroneController(std::shared_ptr<Transform>& theDrone, GameWindow* g)
	: drone(theDrone)
	, gameWindow(g)
{
	state.dirAccel = glm::vec3(0.0f);
	state.rotAccel = glm::vec3(0.0f);
	state.dirSpeed = glm::vec3(0.0f);
	state.rotSpeed = glm::vec3(0.0f);

	state.dirAccelMax = glm::vec3(5.0f, 5.0f, 5.0f);
	state.rotAccelMax = glm::vec3(3.0f, 3.0f, 3.0f);
	state.rotSpeedMax = glm::vec3(2.0f, 2.0f, 2.0f);
	state.dirSpeedMax = glm::vec3(6.0f, 6.0f, 6.0f);
}

DroneController::~DroneController()
{
}

void DroneController::update(float dt)
{
	glm::vec3 inputDir;
	glm::vec3 inputRot;
	checkInput(inputDir, inputRot);

	state.dirAccel = inputDir * state.dirAccelMax;
	state.rotAccel = inputRot * state.rotAccelMax;

	// slow down in every direction the player is not currently inputting
	state.dirAccel.x -= flightControlAdjustment(inputDir.x, state.dirSpeed.x, state.dirAccelMax.x);
	state.dirAccel.y -= flightControlAdjustment(inputDir.y, state.dirSpeed.y, state.dirAccelMax.y);
	state.dirAccel.z -= flightControlAdjustment(inputDir.z, state.dirSpeed.z, state.dirAccelMax.z);
	state.rotAccel.y -= flightControlAdjustment(inputRot.y, state.rotSpeed.y, state.rotAccelMax.y);

	// add acceleration to speed
	state.dirSpeed += dt*state.dirAccel;
	state.rotSpeed += dt*state.rotAccel;

	// cap speed
	state.dirSpeed = glm::max(glm::min(state.dirSpeed, state.dirSpeedMax), -state.dirSpeedMax);
	state.rotSpeed = glm::max(glm::min(state.rotSpeed, state.rotSpeedMax), -state.rotSpeedMax);

	auto rotation = glm::angleAxis(state.rotSpeed.y * dt, glm::vec3(0.0, 1.0, 0.0));
	// rotate speed vector based on rotation so we don't ignore momentum
	state.dirSpeed = state.dirSpeed * rotation;
	drone->translateLocal((state.dirSpeed * dt));
	drone->rotateLocal(rotation);

	//std::cout << "dirSpeed: " << state.dirSpeed.x << " " << state.dirSpeed.y << " " << state.dirSpeed.z << "\n";
}

float DroneController::flightControlAdjustment(float input, float speed, float accelerationMax)
{
	if (input != 0 || speed == 0) return 0.0f;
	return (speed > 0 ? 1.0 : -1.0) * custom_min(accelerationMax, std::fabs(speed));
}

void DroneController::checkInput(glm::vec3& out_dir, glm::vec3& out_rot)
{
	out_dir = glm::vec3(0.0f);
	out_rot = glm::vec3(0.0f);

	if (gameWindow->getInput().getKeyState(Key::W) == KeyState::Pressed) out_dir.z = 1.0;
	if (gameWindow->getInput().getKeyState(Key::S) == KeyState::Pressed) out_dir.z = -1.0;
	if (gameWindow->getInput().getKeyState(Key::A) == KeyState::Pressed) out_dir.x = 1.0;
	if (gameWindow->getInput().getKeyState(Key::D) == KeyState::Pressed) out_dir.x = -1.0;

	if (gameWindow->getInput().getKeyState(Key::LeftShift) == KeyState::Pressed) out_dir.y = 1.0;
	if (gameWindow->getInput().getKeyState(Key::LeftCtrl) == KeyState::Pressed) out_dir.y = -1.0;

	if (gameWindow->getInput().getKeyState(Key::Q) == KeyState::Pressed) out_rot.y = 1.0;
	if (gameWindow->getInput().getKeyState(Key::E) == KeyState::Pressed) out_rot.y = -1.0;
}
