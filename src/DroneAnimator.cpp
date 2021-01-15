#include "DroneAnimator.h"
#include <iostream>
#include <cmath>


DroneAnimator::DroneAnimator(std::shared_ptr<DroneController>& droneController, std::shared_ptr<Transform>& parent_, std::shared_ptr<Transform>& rotor_FL, std::shared_ptr<Transform>& rotor_FR, std::shared_ptr<Transform>& rotor_BL, std::shared_ptr<Transform>& rotor_BR)
	: controller(droneController)
	, parent(parent_)
	, rotorFL(rotor_FL)
	, rotorFR(rotor_FR)
	, rotorBL(rotor_BL)
	, rotorBR(rotor_BR)
	, controllerState(droneController->getState())
{
	state.rotorSpeed = 79.0f;
	state.anglePercentPerSecond = glm::vec3(3.0f, 1.0f, 3.0f);
	state.currentState = glm::vec3(0.0f);
	state.anglesMax = glm::vec3(glm::radians(45.0f), glm::radians(20.0f), glm::radians(45.0f));
}

DroneAnimator::~DroneAnimator()
{
}

void DroneAnimator::update(float dt)
{
	// Blade Rotation
	auto bladeRotation = glm::angleAxis(state.rotorSpeed * dt, glm::vec3(0.0, 1.0, 0.0));
	auto bladeRotationNeg = glm::angleAxis(-state.rotorSpeed * dt, glm::vec3(0.0, 1.0, 0.0));
	rotorFL->rotateLocal(bladeRotationNeg);
	rotorFR->rotateLocal(bladeRotation);
	rotorBL->rotateLocal(bladeRotationNeg);
	rotorBR->rotateLocal(bladeRotation);

	// Body Stuff
	float requiredPitch	= controllerState.dirAccel.z / controllerState.dirAccelMax.z;
	float requiredYaw	= controllerState.rotAccel.y / controllerState.rotAccelMax.y;
	float requiredRoll	= controllerState.dirAccel.x / controllerState.dirAccelMax.x;
	

	state.currentState.x = calculateNewCurrentAngle(state.currentState.x, state.anglePercentPerSecond.x, requiredPitch, dt);
	state.currentState.y = calculateNewCurrentAngle(state.currentState.y, state.anglePercentPerSecond.y, requiredYaw, dt);
	state.currentState.z = calculateNewCurrentAngle(state.currentState.z, state.anglePercentPerSecond.z, requiredRoll, dt);

	auto newPitch = glm::angleAxis(state.currentState.x * state.anglesMax.x, glm::vec3(1.0, 0.0, 0.0));
	auto newYaw = glm::angleAxis(state.currentState.y * state.anglesMax.y, glm::vec3(0.0, 1.0, 0.0));
	auto newRoll = glm::angleAxis(state.currentState.z * state.anglesMax.z, glm::vec3(0.0, 0.0, -1.0));
	auto totalRotation = newPitch * newYaw * newRoll;

	parent->setRotation(totalRotation);
}

float DroneAnimator::calculateNewCurrentAngle(float current /* [-1;+1] */, float perSecond /* percent of max */, float required /* [-1;+1] */, float dt)
{
	float distanceCurrReq = std::fabs(current - required);
	float maxThisFrame = perSecond * dt;
	
	// attempt at smoothing
	// first figuring out how far the current angle is from the required one on a scale from 0 to 1, 1 being the farthest away possible
	// then using the below formular to get a multiplier:
	// arctan(x^2*25)*0.65
	// I suggest you use a graphing tool to visualize it.
	// for example: https://www.desmos.com/calculator
	float relativeDistanceFromTarget = distanceCurrReq / 2.0f;
	maxThisFrame *= std::atan(relativeDistanceFromTarget * relativeDistanceFromTarget * 25.0f) * 0.65f;

	float actualChange = custom_min(distanceCurrReq, maxThisFrame);
	float newCurrent = current - (current - required < 0 ? -1.0f : 1.0f) * actualChange;

	return newCurrent;
}
