#include "Transform.h"
#include <iostream>


void Transform::updateTransformMatrix()
{
	if (!m_matrixDirty) return;
	if(m_parent)
		m_transformMatrix = m_parent->getTransformMatrix() * glm::translate(m_position) * glm::toMat4(m_rotation) * glm::scale(m_scale);
	else 
		m_transformMatrix = glm::translate(m_position) * glm::toMat4(m_rotation) * glm::scale(m_scale);

	m_xaxis = glm::normalize(glm::vec3(m_transformMatrix[0]));
	m_yaxis = glm::normalize(glm::vec3(m_transformMatrix[1]));
	m_zaxis = glm::normalize(glm::vec3(m_transformMatrix[2]));

	m_matrixDirty = false;
}

Transform::Transform() :
	m_position(0.0f),
	m_rotation(1, 0, 0, 0),
	m_scale(1.0f),
	m_transformMatrix(),
	m_xaxis(1.0f, 0.0f, 0.0f),
	m_yaxis(0.0f, 1.0f, 0.0f),
	m_zaxis(0.0f, 1.0f, 0.0f)
{
}

Transform::Transform(const glm::mat4& transformMatrix) :
	m_position(transformMatrix[3]),
	m_rotation(glm::quat_cast(transformMatrix)),
	m_scale(glm::length(glm::vec3(transformMatrix[0])), glm::length(glm::vec3(transformMatrix[1])), glm::length(glm::vec3(transformMatrix[2]))),
	m_transformMatrix(transformMatrix),
	m_xaxis(glm::normalize(glm::vec3(transformMatrix[0]))),
	m_yaxis(glm::normalize(glm::vec3(transformMatrix[1]))),
	m_zaxis(glm::normalize(glm::vec3(transformMatrix[2])))
{
}

Transform::Transform(const glm::vec3& position, const glm::quat& rotation, const glm::vec3& scale) :
	m_position(position),
	m_rotation(rotation),
	m_scale(scale),
	m_transformMatrix(glm::translate(position)* glm::toMat4(rotation)* glm::scale(scale)),
	m_xaxis(glm::normalize(glm::vec3(toMat3(rotation)[0]))),
	m_yaxis(glm::normalize(glm::vec3(toMat3(rotation)[1]))),
	m_zaxis(glm::normalize(glm::vec3(toMat3(rotation)[2])))
{}

const glm::vec3& Transform::getPosition()
{
	return m_position;
}

const glm::quat& Transform::getRotation()
{
	return m_rotation;
}

const glm::vec3& Transform::getScale()
{
	return m_scale;
}

void Transform::setPosition(const glm::vec3& position)
{
	m_position = position;
	markDirty();
}

void Transform::setRotation(const glm::quat& rotation)
{
	m_rotation = rotation;
	markDirty();
}

void Transform::setScale(const glm::vec3& scale)
{
	m_scale = scale;
	markDirty();
}

const glm::mat4& Transform::getMatrix()
{
	updateTransformMatrix();
	return m_transformMatrix;
}

void Transform::setMatrix(const glm::mat4& matrix)
{
	Transform tmp(matrix);
	m_position = tmp.m_position;
	m_rotation = tmp.m_rotation;
	m_scale = tmp.m_scale;
	m_transformMatrix = matrix;
	m_xaxis = tmp.m_xaxis;
	m_yaxis = tmp.m_yaxis;
	m_zaxis = tmp.m_zaxis;
	markDirty();
}

void Transform::translate(const glm::vec3& deltaPos)
{
	m_position += deltaPos;
	markDirty();
}

void Transform::translateLocal(const glm::vec3& deltaPos)
{
	m_position += deltaPos.x * m_xaxis + deltaPos.y * m_yaxis + deltaPos.z * m_zaxis;
	markDirty();
}

void Transform::rotate(const glm::quat& deltaRot)
{

	m_rotation = glm::normalize(deltaRot * m_rotation);
	markDirty();

}

void Transform::rotateLocal(const glm::quat& deltaRot)
{
	m_rotation = glm::normalize(m_rotation * deltaRot);
	markDirty();
}

void Transform::scale(const glm::vec3& scale)
{
	m_scale *= scale;
	markDirty();
}

const glm::vec3& Transform::getXAxis()
{
	updateTransformMatrix();
	return m_xaxis;
}

const glm::vec3& Transform::getYAxis()
{
	updateTransformMatrix();
	return m_yaxis;
}

const glm::vec3& Transform::getZAxis()
{
	updateTransformMatrix();
	return m_zaxis;
}

const glm::mat4& Transform::getTransformMatrix()
{
	updateTransformMatrix();
	return m_transformMatrix;
}

glm::vec3 Transform::getDirection()
{
	return -getZAxis();
}

void Transform::lookinto(const glm::vec3& direction, const glm::vec3& trueUp)
{
	glm::vec3 ndir(glm::normalize(-direction));
	glm::vec3 right = glm::normalize(glm::cross(trueUp, ndir));
	glm::vec3 up = glm::normalize(glm::cross(ndir, right));

	//construct a 3x3 rotation matrix from direction and global up vector
	glm::mat3 rot(right, up, ndir);

	setRotation(glm::quat_cast(rot));
	markDirty();
}

void Transform::lookat(const glm::vec3& point, const glm::vec3& up)
{
	//auto result = glm::lookAt(getPosition(), point, up);
	updateTransformMatrix();
	auto direction = glm::normalize(point - getPosition());
	lookinto(direction, up);
}

glm::mat4 Transform::getInverseMatrix()
{
	updateTransformMatrix();
	return glm::inverse(m_transformMatrix);
}

void Transform::setParent(Transform* parent) {
	m_parent = parent;
	if(m_parent)
		m_parent->addChild(this);
	markDirty();
}

Transform* Transform::getParent() {
	return m_parent;
}

void Transform::addChild(Transform* child)
{
	if(child)
		m_children.insert(child);
}

void Transform::removeChild(Transform* child)
{
	m_children.erase(child);
}

void Transform::markDirty()
{
	m_matrixDirty = true;
	for (auto child : m_children)
		child->markDirty();
}

Transform::~Transform()
{
	if (m_parent)
		m_parent->removeChild(this);
	for (auto& child : m_children) {
		child->setParent(m_parent);
	}
}
