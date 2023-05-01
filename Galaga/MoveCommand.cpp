#include "MoveCommand.h"
#include "GameObject.h"
#include <iostream>



MoveCommand::MoveCommand(engine::GameObject* actor, glm::vec3 direction, float speed)
	: Command()
	, m_Actor(actor)
	, m_Speed{ speed }	
{
	m_Direction = glm::normalize(direction);   // Normalized vector with the same direction but with lenght = 1
}

void MoveCommand::Execute(float deltaTime)
{
	auto transformCP = m_Actor->GetComponent<engine::TransformComponent>();
	
	if (transformCP)
	{
		glm::vec3 pos = transformCP->GetLocalPosition();

		pos += m_Direction * m_Speed * deltaTime;

		transformCP->SetLocalPosition(pos);
	}
	
}


MoveCommand::~MoveCommand()
{
	std::cout << "MoveCommand destructor" << std::endl;
}