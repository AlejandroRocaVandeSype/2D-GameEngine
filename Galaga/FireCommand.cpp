#include "FireCommand.h"
#include "GameObject.h"
#include "MissileManagerCP.h"


FireCommand::FireCommand(engine::GameObject* actor, glm::vec3 direction)
	: Command()
	, m_Actor(actor)
{
	m_Direction = glm::normalize(direction);   // Normalized vector with the same direction but with lenght = 1
}

FireCommand::~FireCommand()
{

}

void FireCommand::Execute(float)
{
	if (m_Actor->IsActive())
	{
		auto missileManagerCP = m_Actor->GetComponent<MissileManagerCP>();
		if (missileManagerCP != nullptr)
		{
			missileManagerCP->Fire(m_Direction);
		}
	}	
}
