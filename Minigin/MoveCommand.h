#pragma once
#include "Command.h"
#include <glm/glm.hpp>

class MoveCommand final : public Command
{
public:
	explicit MoveCommand(dae::GameObject* actor, glm::vec3 direction, float speed);

	void Execute(float deltaTime) override;


private :
	float m_Speed;
	glm::vec3 m_Direction;		// Normalized vector that indicates the direction of the movement
};

