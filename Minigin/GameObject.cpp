#include "GameObject.h"
#include <iostream>

using namespace dae;

GameObject::GameObject(glm::vec3 startPosition)
	: m_IsActive{ true }
	, m_IsDead { false }
	, m_HasToRender { false }
	, m_pRenderCP{ nullptr }
{
	// All gameObjects have a transform component attach when created
	m_pTransformCP = AddComponent<TransformComponent>(this, startPosition);
	
}

GameObject::~GameObject()
{
	std::cout << "GameObject destructor" << std::endl;
	
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
	// TODO: Check who sent the message to not send it him again
	for (auto& componentItr : m_vComponents)
	{
		componentItr->ReceiveMessage(message, value);
	}
}


const bool GameObject::HasARender() const
{
	return m_HasToRender;
}

const bool GameObject::IsMarkedAsDead() const
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