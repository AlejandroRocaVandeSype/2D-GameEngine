#include "MoveCommand.h"
#include "GameObject.h"
#include <iostream>

MoveCommand::MoveCommand(dae::GameObject* actor, glm::vec3 direction, float speed)
	: Command(actor)
	, m_Speed{ speed }	
{
	m_Direction = glm::normalize(direction);   // Normalized vector with the same direction but with lenght = 1
}

void MoveCommand::Execute([[maybe_unused]]float deltaTime)
{
	auto transformCP = GetActor()->GetComponent<TransformComponent>();
	
	if (transformCP)
	{
		glm::vec3 pos = transformCP->GetLocalPosition();

		pos += m_Direction * m_Speed * deltaTime;

		transformCP->SetLocalPosition(pos);
	}
	
}