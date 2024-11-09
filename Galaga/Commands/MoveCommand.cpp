#include "MoveCommand.h"
#include "GameObject.h"
#include "../Components/General/MoveComponent.h"
#include "glm/glm.hpp"

MoveCommand::MoveCommand(engine::GameObject* actor, glm::vec3 direction)
	: Command()
	, m_Actor(actor)
{
	m_Direction = glm::normalize(direction);   // Normalized vector with the same direction but with lenght = 1
}

void MoveCommand::Execute(float deltaTime)
{
	if (m_Actor->IsActive())
	{
		auto moveCP = m_Actor->GetComponent<MoveComponent>();
		if (moveCP != nullptr)
		{
			moveCP->Move(deltaTime, m_Direction);
		}
	}	
}


MoveCommand::~MoveCommand()
{
}