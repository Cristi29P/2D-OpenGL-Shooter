#include "lab_m1/shooter/bullet.h"

bullet::bullet() {
}

bullet::~bullet() {
}

void bullet::setModelMatrix(glm::mat3 newModelMatrix) {
	m_modelMatrix = newModelMatrix;
}

void bullet::setInitialPosition(glm::vec3 newInitialPosition) {
	m_initialPosition = newInitialPosition;
}

void bullet::setPosition(glm::vec3 newPosition) {
	m_position = newPosition;
}
void bullet::setAngle(float newAngle) {
	m_angle = newAngle;
}

void bullet::setBulletSpeed(float newBulletSpeed) {
	m_bulletSpeed = newBulletSpeed;
}

void bullet::setDirection(glm::vec3 newDirection) {
	m_direction = newDirection;
}

void bullet::setDistance(float newDistance) {
	m_distance = newDistance;
}

void bullet::update(float deltaTimeSeconds) {
	translateX += deltaTimeSeconds * m_direction.x * m_bulletSpeed;
	translateY += deltaTimeSeconds * m_direction.y * m_bulletSpeed;
	// Compute the new model matrix
	m_modelMatrix =
		transform2D::Translate(translateX, translateY) *
		transform2D::Translate(m_initialPosition.x, m_initialPosition.y) *
		transform2D::Rotate(m_angle) *
		transform2D::Translate(-0.1f, -0.1f);

	// Compute the total travelled distance
	m_distance += (float)sqrt(pow(m_direction.x * deltaTimeSeconds * m_bulletSpeed, 2) +
		pow(m_direction.y * deltaTimeSeconds * m_bulletSpeed, 2));

	// Update the internal position with regard to map
	m_position = m_modelMatrix * glm::vec3(0.1f, 0.1f, 1.f);
}