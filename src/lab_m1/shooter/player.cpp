#include "lab_m1/shooter/player.h"

player::player() {
}

player::~player() {
}

player::player(glm::vec3 initialPosition) {
	m_initialPosition = initialPosition;
}

void player::setModelMatrix(glm::mat3 newModelMatrixBody) {
	m_modelMatrixBody = newModelMatrixBody;
}

void player::setPosition(glm::vec3 newPosition) {
	m_position = newPosition;
}
void player::setAngle(float newAngle) {
	m_angle = newAngle;
}
void player::setHealth(float newHealth) {
	m_health = newHealth;
}

void player::setFirerate(float newFirerate) {
	m_fireRate = newFirerate;
}

void player::setBulletSpeed(float newBulletSpeed) {
	m_bulletSpeed = newBulletSpeed;
}

void player::setProjectileRange(float newProjectileRange) {
	m_projectileRange = newProjectileRange;
}

void player::increasePoints(int pointsAdded) {
	m_points += pointsAdded;
}

void player::updatePlayer(float trX, float trY) {
	// Update the modelMatrix
	m_modelMatrixBody =
		transform2D::Translate(4.f + trX, 2.25f + trY) *
		transform2D::Rotate(m_angle);

	// Update the internal position with regard to map
	m_position = m_modelMatrixBody * m_initialPosition;
}