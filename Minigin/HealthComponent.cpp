#include "HealthComponent.h"
#include <iostream>

HealthComponent::HealthComponent(dae::GameObject* pOwner, unsigned int lives)
	: Component("HealthCP", pOwner)
	, m_Lives { lives }
{

}

HealthComponent::~HealthComponent()
{
	std::cout << "Health Component destructor" << std::endl;
}

void HealthComponent::Update([[maybe_unused]] const float deltaTime)
{

}

void HealthComponent::DecrementHealth(unsigned int amount)
{
	if (amount > m_Lives)
	{
		m_Lives = 0;
	}
	else
	{
		m_Lives -= amount;
	}
}

void HealthComponent::ReceiveMessage([[maybe_unused]] const std::string& message, [[maybe_unused]] const std::string& value)
{

}


const unsigned int HealthComponent::GetLives() const
{
	return m_Lives;
}