#pragma once

#include "lab_m1/shooter/transform2D.h"

class enemy {
private:
	glm::mat3 m_modelMatrix = glm::mat3(1);
	glm::vec3 m_position = glm::vec3(0.f, 0.f, 1);
	glm::vec3 m_initialPosition = glm::vec3(0.f, 0.f, 1);
	glm::vec3 m_direction = glm::vec3(0.f, 0.f, 0.f);

	float m_speed = 0;
	float translateX = 0, translateY = 0;

public:
	enemy(glm::vec3 position, float speed);
	~enemy();

public:

	glm::mat3 getModelMatrix() { return m_modelMatrix; }
	void setModelMatrix(glm::mat3 newModelMatrix);

	glm::vec3 getPosition() { return m_position; }
	void setPosition(glm::vec3 newPosition);

	void update(float deltaTimeSeconds, float new_angle, glm::vec2 new_direction);
};