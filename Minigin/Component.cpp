#include "Component.h"


Component::Component(ComponentType type)
	: m_ComponentType{ type }
{

}


const Component::ComponentType Component::Type() const
{
	return m_ComponentType;
}