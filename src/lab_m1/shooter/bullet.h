#pragma once

#include "lab_m1/shooter/transform2D.h"

class bullet {
private:
	glm::mat3 m_modelMatrix = glm::mat3(1);
	glm::vec3 m_position = glm::vec3(0, 0, 1);
	glm::vec3 m_initialPosition = glm::vec3(0, 0, 1);
	glm::vec3 m_direction = glm::vec3(0, 0, 0);

	float m_angle = 0.f;
	float m_distance = 0.f;
	float m_bulletSpeed = 0.f;
	float translateX = 0.f, translateY = 0.f;

public:
	bullet();
	~bullet();

public:
	glm::mat3 getModelMatrix() { return m_modelMatrix; }
	void setModelMatrix(glm::mat3 newModelMatrix);

	glm::vec3 getPosition() { return m_position; }
	void setPosition(glm::vec3 newPosition);

	glm::vec3 getInitialPosition() { return m_initialPosition; }
	void setInitialPosition(glm::vec3 newInitialPosition);

	void setDirection(glm::vec3 newDirection);

	float getAngle() { return m_angle; }
	void setAngle(float newAngle);

	float getBulletSpeed() { return m_bulletSpeed; }
	void setBulletSpeed(float newBulletSpeed);

	float getDistance() { return m_distance; }
	void setDistance(float newDistance);

	void update(float deltaTimeSeconds);
};