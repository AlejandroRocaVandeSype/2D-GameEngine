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
	// When gameObject is created, WorldPos = localPos since it wont have a parent at start
	m_pTransformCP = AddComponent<TransformComponent>(this, startPosition);

	SetParent(nullptr);
	
}

GameObject::~GameObject()
{
	std::cout << "GameObject destructor" << std::endl;	
	// TODO : Remove parents from all the children
	if (m_pParent != nullptr)
	{
		SetParent(nullptr);
	}

	if (!m_vChildren.empty())
	{
		for (auto& child : m_vChildren)
		{
			child->SetParent(nullptr);
		}
	}

	m_pTransformCP = nullptr;
	m_pRenderCP = nullptr;
	
}

void GameObject::Update(const float deltaTime)
{
	// TODO : Check if components is marked as dead and remove at the end
	if (m_IsActive)  // Only if the object is active we update
	{
		for (auto& componentItr : m_vComponents)
		{
			componentItr->Update(deltaTime);
		}
	}
	
}

std::vector<GameObject*>& GameObject::getChildren()
{
	return m_vChildren;
}

void GameObject::UpdateChildrenPosition()
{
	// Set position is dirty to all children in order to update its positions
	for (auto& child : m_vChildren)
	{
		child->SetPositionDirty();
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

// Updates the parent of the gameObject
// If pNewParent = nullptr --> We want to remove the current parent (if it has) and 
// we use removeChild to remove it from the children container
void GameObject::SetParent(GameObject* pNewParent, bool keepWorldPosition)
{
	// FIRST UPDATE THE LOCAL POSITION OF THE GAMEOBJECT
	// TODO : Update scale and rotation too
	if (pNewParent == nullptr)
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
				m_pTransformCP->SetLocalPosition(m_pTransformCP->GetLocalPosition() - pNewParent->GetWorldPosition());
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
	m_pParent = pNewParent;

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

void GameObject::RemoveChild(GameObject* child)
{
	auto childItr = std::find(m_vChildren.begin(), m_vChildren.end(), child);

	if (childItr != m_vChildren.end())
	{
		// Found
		m_vChildren.erase(childItr);
	}
}

void GameObject::AddChild(GameObject* child)
{
	m_vChildren.push_back(child);
}

const glm::vec3 GameObject::GetWorldPosition() const
{
	// TODO : What if the transformCP doesnt exit?
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

void GameObject::SetPositionDirty()
{
	if (m_pTransformCP != nullptr)
	{
		m_pTransformCP->SetPositionDirty();
	}
}