#include "lab_m1/shooter/enemy.h"

enemy::enemy(glm::vec3 initialPosition, float speed) {
	m_initialPosition = initialPosition;
	m_speed = speed;
}
enemy::~enemy() {
}

void enemy::setModelMatrix(glm::mat3 newModelMatrix) {
	m_modelMatrix = newModelMatrix;
}

void enemy::setPosition(glm::vec3 newPosition) {
	m_position = newPosition;
}

void enemy::update(float deltaTimeSeconds, float new_angle, glm::vec2 new_direction) {
	translateX += m_speed * deltaTimeSeconds * new_direction.x;
	translateY += m_speed * deltaTimeSeconds * new_direction.y;
	// Compute the new model matrix
	m_modelMatrix =
		transform2D::Translate(translateX, translateY) *
		transform2D::Translate(m_initialPosition.x, m_initialPosition.y) *
		transform2D::Rotate(new_angle);

	// Update the internal position
	m_position = m_modelMatrix * glm::vec3(0.f, 0.f, 1.f);
}