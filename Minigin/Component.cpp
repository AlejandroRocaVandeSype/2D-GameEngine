#include "Component.h"
#include "GameObject.h"


engine::Component::Component(const std::string& name, engine::GameObject* pOwner)
	: m_Tag { name }
	, m_pOwner { pOwner }
{

}
