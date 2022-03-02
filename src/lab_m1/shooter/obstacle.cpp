#include "lab_m1/shooter/obstacle.h"

obstacle::obstacle(float sideLength, float sX, float sY, glm::vec3 position)
{
	m_sideLength = sideLength;
	m_sX = sX;
	m_sY = sY;
	m_position = position;

	// Compute the modelMatrix during the init phase
	m_modelMatrix =
		transform2D::Translate(position.x, position.y) *
		transform2D::Scale(sX, sY) *
		transform2D::Translate(-sideLength / 2, -sideLength / 2) *
		glm::mat3(1);
}

obstacle::~obstacle() {
}