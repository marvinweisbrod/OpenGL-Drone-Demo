#include "DroneController.h"
#include <iostream>
#include <cmath>

DroneController::DroneController(std::shared_ptr<Transform>& theDrone, GameWindow* g, std::vector<std::shared_ptr<Renderable>>& b)
	: drone(theDrone)
	, gameWindow(g)
	, buildings(b)
{
	state.dirAccel = glm::vec3(0.0f);
	state.rotAccel = glm::vec3(0.0f);
	state.dirSpeed = glm::vec3(0.0f);
	state.rotSpeed = glm::vec3(0.0f);

	state.dirAccelMax = glm::vec3(5.0f, 5.0f, 5.0f);
	state.rotAccelMax = glm::vec3(3.0f, 3.0f, 3.0f);
	state.rotSpeedMax = glm::vec3(2.0f, 2.0f, 2.0f);
	state.dirSpeedMax = glm::vec3(6.0f, 6.0f, 6.0f);

	initialRotation = drone->getRotation();
	initialPosition = drone->getPosition();
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

	glm::vec4 previousPos = drone->getTransformMatrix() * glm::vec4(0.0f,0.0f,0.0f,1.0f);


	// Disallow going below Y=0.2
	const float buffer = 0.2f;
	if (previousPos.y - buffer < 0.0f && state.dirSpeed.y < 0)
		state.dirSpeed.y = 0;
	glm::vec3 frameMovement = state.dirSpeed * dt;
	if (frameMovement.y < 0 && previousPos.y + frameMovement.y < 0) {
		frameMovement.y = frameMovement.y + (previousPos.y + frameMovement.y);
		state.dirSpeed.y = 0;
	}

	drone->translateLocal(frameMovement);
	drone->rotateLocal(rotation);

	glm::vec3 newPos(drone->getTransformMatrix() * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
	// check for collisions
	if (isColliding(newPos)) {
		state.dirSpeed = glm::vec3(0.0f);
		state.dirAccel = glm::vec3(0.0f);
		drone->translateLocal(-frameMovement);
		drone->rotateLocal(-rotation);
	}
}

float DroneController::flightControlAdjustment(float input, float speed, float accelerationMax)
{
	if (input != 0 || speed == 0) return 0.0f;
	return (speed > 0 ? 1.0f : -1.0f) * custom_min(accelerationMax, std::fabs(speed));
}

void DroneController::reset()
{
	state.dirSpeed = glm::vec3(0.0f);
	state.dirAccel = glm::vec3(0.0f);
	state.rotSpeed = glm::vec3(0.0f);
	state.rotAccel = glm::vec3(0.0f);
	drone->setPosition(initialPosition);
	drone->setRotation(initialRotation);
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

bool DroneController::isColliding(glm::vec3& newPos)
{
	for (auto& building : buildings) {
		auto bounds = building->getTransformedBounds();
		auto min = bounds.getMin();
		auto max = bounds.getMax();
		if (min.x < newPos.x && max.x > newPos.x
			&& min.y < newPos.y && max.y > newPos.y
			&& min.z < newPos.z && max.z > newPos.z)
			return true;
	}
	return false;
}
