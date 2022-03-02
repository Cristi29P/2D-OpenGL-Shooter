#pragma once

#include "lab_m1/shooter/transform2D.h"

class player {
private:
	glm::mat3 m_modelMatrixBody = glm::mat3(1);
	glm::vec3 m_initialPosition = glm::vec3(0.f, 0.f, 1.f);
	glm::vec3 m_position = glm::vec3(0.f, 0.f, 1.f);

	float m_angle = 0.f;
	float m_health = 0.f;
	float m_fireRate = 0.f;
	float m_projectileRange = 0.f;
	float m_bulletSpeed = 0.f;
	int m_points = 0;

public:
	player();
	player(glm::vec3 initialPosition);
	~player();

public:

	glm::mat3 getModelMatrix() { return m_modelMatrixBody; }
	void setModelMatrix(glm::mat3 newModelMatrixBody);

	glm::vec3 getPosition() { return m_position; }
	void setPosition(glm::vec3 newPosition);

	float getAngle() { return m_angle; }
	void setAngle(float newAngle);

	float getBulletSpeed() { return m_bulletSpeed; }
	void setBulletSpeed(float newBulletSpeed);

	float getHealth() { return m_health; }
	void setHealth(float newHealth);

	float getFirerate() { return m_fireRate; }
	void setFirerate(float newFirerate);

	float getProjectileRange() { return m_projectileRange; }
	void setProjectileRange(float newProjectileRange);

	int getPoints() { return m_points; }
	void increasePoints(int pointsAdded);

	void updatePlayer(float trX, float trY);
};