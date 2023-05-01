#include "Component.h"
#include "GameObject.h"


engine::Component::Component(const std::string& name, engine::GameObject* pOwner)
	: m_Name { name }
	, m_pOwner { pOwner }
{

}
