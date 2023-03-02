#include "GameObject.h"
#include <iostream>

using namespace dae;

GameObject::GameObject()
	: m_IsActive{ true }
	, m_IsDead { false }
	, m_HasToRender { false }
	, m_pRenderCP{ nullptr }
{
	// All gameObjects have a transform component attach when created
	//m_pTransformCP = std::make_shared<TransformComponent>(AddComponent<TransformComponent>());
	m_pTransformCP = AddComponent<TransformComponent>();
	
}

GameObject::~GameObject()
{
	std::cout << "GameObject destructor" << std::endl;
	
	for (auto& componentItr : m_vComponents)
	{
		delete componentItr;
		componentItr = nullptr;
	}
	m_vComponents.clear();

	m_pTransformCP = nullptr;
	m_pRenderCP = nullptr;
	
}

void GameObject::Update(const float deltaTime)
{
	if (m_IsActive)  // Only if the object is active we update
	{
		for (auto& componentItr : m_vComponents)
		{
			componentItr->Update(deltaTime);
		}
	}
	
}

void GameObject::Render() const
{
	if (m_IsActive)
	{
		if (HasARender() && m_pRenderCP != nullptr && m_pTransformCP != nullptr)
		{
			m_pRenderCP->Render(m_pTransformCP->GetPosition());
		}
	}
	
}

// Send a message to all the components
void GameObject::SendMessage(const std::string& message, const std::string& value)
{
	// Check who sent the message to not send it him again
	for (auto& componentItr : m_vComponents)
	{
		componentItr->ReceiveMessage(message, value);
	}
}


void GameObject::Destroy()
{
	
}

const bool GameObject::HasARender() const
{
	return m_HasToRender;
}

const bool GameObject::IsDead() const
{
	return m_IsDead;
}

void GameObject::SetIsActive(const bool isActive)
{
	m_IsActive = isActive;
}

void GameObject::SetIsDead(const bool isDead)
{
	m_IsDead = isDead;
}