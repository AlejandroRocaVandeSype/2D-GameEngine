#include "GameObject.h"
#include <iostream>

using namespace dae;

GameObject::GameObject(glm::vec3 startPosition)
	: m_pParent { nullptr }
	, m_IsActive{ true }
	, m_IsDead { false }
	, m_HasToRender { false }
	, m_pRenderCP{ nullptr }
{
	// All gameObjects have a transform component attach when created
	m_pTransformCP = AddComponent<TransformComponent>(this, startPosition);

	SetParent(nullptr);
	
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
			m_pRenderCP->Render(m_pTransformCP->GetWorldPosition());
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

void GameObject::SetParent(GameObject* parent, bool keepWorldPosition)
{
	// FIRST UPDATE THE LOCAL POSITION OF THE GAMEOBJECT
	if (parent == nullptr)
	{
		// This gameObject wont have parent --> LocalPosition = WorldPosition
		if (m_pTransformCP != nullptr)
		{
			m_pTransformCP->SetLocalPosition(m_pTransformCP->GetWorldPosition());
		}
	}
	else
	{
		// Check if we want to move the gameObject to its parent worldPosition
		if (keepWorldPosition)
		{
			if (m_pTransformCP != nullptr)
			{
				m_pTransformCP->SetLocalPosition(m_pTransformCP->GetLocalPosition() - parent->GetWorldPosition());
			}
		}

		if (m_pTransformCP != nullptr)
		{
			m_pTransformCP->SetPositionDirty();
		}
		
	}

	// NOW MAKE THE SCENEGRAPH HIERARCHY 
	if (m_pParent != nullptr)
	{
		// This gameObject already has a parent -> Remove itself as a child 
		m_pParent->RemoveChild(this);		
	}
	m_pParent = parent;

	// If we are adding a new parent to the gameObject then we need to add this as a child
	if (m_pParent != nullptr)
	{
		m_pParent->AddChild(this);
	}

}

const GameObject* GameObject::getParent() const
{
	return m_pParent;
}

void GameObject::RemoveChild([[maybe_unused]] GameObject* child)
{

}
void GameObject::AddChild([[maybe_unused]] GameObject* child)
{

}

const glm::vec3 GameObject::GetWorldPosition() const
{
	return m_pTransformCP->GetWorldPosition();
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