#pragma once

#include "Transform.h"
#include <libheaders.h>

class Camera : public Transform {
public:
	Camera();
	glm::mat4 getViewMatrix();
	glm::mat4 getProjectionMatrix();

	void setPerspective(float theta, float aspect, float near, float far);
private:
	glm::mat4 perspective;
};