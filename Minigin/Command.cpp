#include "Command.h"
#include "GameObject.h"

Command::Command(dae::GameObject* actor)
	: m_Actor { actor }
{

}