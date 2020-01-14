#pragma once
#include "Component.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/mat4x4.hpp>


class TransformComponent : public Component
{
public:

	glm::vec3 x_axis = glm::vec3(0.1, 0, 0);
	glm::vec3 y_axis = glm::vec3(0, 0.1, 0);
	glm::vec3 z_axis = glm::vec3(0, 0, 0.1);

	glm::vec3 m_position;
	glm::quat m_orientation;
	glm::vec3 m_scale;

	void OnUpdate(float dt) override{}
	void OnMessage(const std::string m) override
	{
		if (m == "rotateleft") {
			rotate(0.01, glm::vec3(0, -1, 0));
		}
		if (m == "rotateright") {
			rotate(0.01, glm::vec3(0, 1, 0));
		}
		if (m == "rotateup") {
			rotate(0.01, glm::vec3(1, 0, 0));
		}
		if (m == "rotatedown") {
			rotate(0.01, glm::vec3(-1, 0, 0));
		}
		if (m == "moveleft") {
			translate(-x_axis * glm::inverse(m_orientation));
		}
		if (m == "moveforward") {
			translate(-z_axis * glm::inverse(m_orientation));
		}
		if (m == "moveright") {
			translate(x_axis* glm::inverse(m_orientation));
		}
		if (m == "movebackward") {
			translate(z_axis * glm::inverse(m_orientation));
		}
	}
	TransformComponent() : m_position(0), m_orientation(1,0,0,0), m_scale(1.0f) {}
	TransformComponent(const glm::vec3& pos) : m_position(pos), m_orientation(1, 0, 0, 0), m_scale(1.0f) {}
	TransformComponent(const glm::vec3& pos, const glm::quat& orient) : m_position(pos), m_orientation(orient), m_scale(1.0f) {}
	TransformComponent(const glm::vec3& pos, const glm::quat& orient, const glm::vec3& scale) : m_position(pos), m_orientation(orient), m_scale(scale) {}

	const glm::vec3& position() const { return m_position; }
	const glm::quat& orientation() const { return m_orientation; }
	const glm::vec3& scale() const { return m_scale; }

	glm::mat4 getModelMatrix()
	{
		glm::mat4 transMatrix = glm::translate(glm::mat4(1.0f), m_position);
		glm::mat4 scaleMatrix = glm::scale(glm::mat4(1.0f), m_scale);
		glm::mat4 rotMatrix = glm::mat4_cast(m_orientation);
		return transMatrix * rotMatrix * scaleMatrix;
	}

	void translate(const glm::vec3 &v) { m_position += v; }
	void translate(float x, float y, float z) { m_position += glm::vec3(x, y, z); }

	void rotate(float angle, const glm::vec3 &axis) { m_orientation *= glm::angleAxis(angle, axis * m_orientation); }
	void rotate(float angle, float x, float y, float z) { m_orientation *= glm::angleAxis(angle, glm::vec3(x, y, z) * m_orientation); }

	void scaleUp(const glm::vec3 &v) { m_scale += v; }
	void scaleUp(float x, float y, float z) { m_scale += glm::vec3(x, y, z); }

	void yaw(float angle) { rotate(angle, 0.0f, 1.0f, 0.0f); }
	void pitch(float angle) { rotate(angle, 1.0f, 0.0f, 0.0f); }
	void roll(float angle) { rotate(angle, 0.0f, 0.0f, 1.0f); }
};
