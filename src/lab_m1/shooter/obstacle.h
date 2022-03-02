#pragma once

#include "lab_m1/shooter/transform2D.h"

class obstacle {
public:
	glm::vec3 m_position = glm::vec3(0.f, 0.f, 1.f);
	glm::mat3 m_modelMatrix = glm::mat3(1);
	float m_sX = 1.f, m_sY = 1.f;
	float m_sideLength = 1.f;

public:
	obstacle(float sideLength, float sX, float sY, glm::vec3 position);
	~obstacle();
};