#pragma once

#include <glm/glm.hpp>

enum VectorDirection : unsigned char
{
	Up = 0,
	Right,
	Down,
	Left
};

inline VectorDirection GetVectorDirection(glm::vec2 vNormalized)
{
	float vDotUp = glm::dot(vNormalized, glm::vec2(0.0f, -1.0f));
	float vDotRight = glm::dot(vNormalized, glm::vec2(1.0f, 0.0f));
	float vDotDown = glm::dot(vNormalized, glm::vec2(0.0f, 1.0f));
	float vDotLeft = glm::dot(vNormalized, glm::vec2(-1.0f, 0.0f));

	float max = vDotUp;
	VectorDirection direction = VectorDirection::Up;

	if (vDotRight > max)
	{
		max = vDotRight;
		direction = VectorDirection::Right;
	}

	if (vDotDown > max)
	{
		max = vDotDown;
		direction = VectorDirection::Down;
	}

	if (vDotLeft > max)
	{
		max = vDotLeft;
		direction = VectorDirection::Left;
	}

	return direction;
}
