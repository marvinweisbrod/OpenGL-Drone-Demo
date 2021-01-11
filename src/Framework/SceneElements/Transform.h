#ifndef _TRANSFORM_H_
#define _TRANSFORM_H_
#include <libheaders.h>
#include <set>

class Transform
{
private:
	Transform* m_parent = nullptr;
	std::set<Transform*> m_children;
	glm::mat4 m_transformMatrix;
	bool m_matrixDirty = true;

	glm::vec3 m_position;
	glm::quat m_rotation;
	glm::vec3 m_scale;

	glm::vec3 m_xaxis;
	glm::vec3 m_yaxis;
	glm::vec3 m_zaxis;

	void updateTransformMatrix();

protected:
	void addChild(Transform* child);
	void removeChild(Transform* child);
	void markDirty();
public:
	Transform();
	~Transform();
	Transform(const Transform& other) = default;
	Transform& operator=(const Transform& other) = default;
	Transform(Transform&& other) = default;
	Transform& operator=(Transform&& other) = default;
	Transform(const glm::mat4& transformMatrix);
	Transform(const glm::vec3& position, const glm::quat& rotation, const glm::vec3& scale);

	void setParent(Transform* parent);
	Transform* getParent();

	const glm::vec3& getPosition();
	const glm::quat& getRotation();
	const glm::vec3& getScale();

	void setPosition(const glm::vec3& position);
	void setRotation(const glm::quat& rotation);
	void setScale(const glm::vec3& scale);

	const glm::mat4& getMatrix();
	void setMatrix(const glm::mat4& matrix);

	void translate(const glm::vec3& deltaPos);
	void translateLocal(const glm::vec3& deltaPos);

	void rotate(const glm::quat& deltaRot);
	void rotateLocal(const glm::quat& deltaRot);

	void scale(const glm::vec3& scale);

	const glm::vec3& getXAxis();
	const glm::vec3& getYAxis();
	const glm::vec3& getZAxis();

	const glm::mat4& getTransformMatrix();

	glm::vec3 getDirection();
	void lookinto(const glm::vec3& direction, const glm::vec3& up = glm::vec3(0.0f,1.0f,0.0f));
	void lookat(const glm::vec3& point, const glm::vec3& up = glm::vec3(0.0f, 1.0f, 0.0f));
	glm::mat4 getInverseMatrix();
};
#endif