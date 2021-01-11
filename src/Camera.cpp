#include "Camera.h"

Camera::Camera()
	: perspective(glm::mat4(1.0f))
{
}

const glm::mat4 Camera::getViewMatrix()
{
	return getInverseMatrix();
}

const glm::mat4 Camera::getProjectionMatrix()
{
	return perspective;
}

void Camera::setPerspective(float fov, float aspect, float near, float far)
{
	perspective = glm::perspective(fov, aspect, near, far);
}
