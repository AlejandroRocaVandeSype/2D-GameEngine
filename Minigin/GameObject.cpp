#include "GameObject.h"
#include <iostream>

using namespace dae;

GameObject::GameObject(glm::vec3 startPosition)
	: m_pParent { nullptr }
	, m_IsActive{ true }
	, m_IsDead { false }
	, m_HasToRender { false }
	, m_pRenderCP{ nullptr }
	, m_TotalDeadChildren { 0 }
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
	
	/*
	if (m_pParent != nullptr)
	{
		SetParent(nullptr);
	}
	*/
	/*
	if (!m_vChildren.empty())
	{
		for (auto& child : m_vChildren)
		{
			child->SetParent(nullptr);
		}
	}
	*/

	m_pTransformCP = nullptr;
	m_pRenderCP = nullptr;
	
}

void GameObject::Update(const float deltaTime)
{
	// TODO : Check if components are marked as dead and remove at the end
	if (m_IsActive)  // Only if the object is active we update
	{
		for (auto& componentItr : m_vComponents)
		{
			componentItr->Update(deltaTime);
		}

		// If this gameObject has children update them too
		if (!m_vChildren.empty())
		{
			for (auto& child : m_vChildren)
			{
				if (!child->IsMarkedAsDead())
				{
					child->Update(deltaTime);
				}
				
			}
		}
	}
	
}
/*
std::vector<GameObject*>& GameObject::getChildren()
{
	return m_vChildren;
}
*/
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

		if (!m_vChildren.empty())
		{
			for (auto& child : m_vChildren)
			{
				child->Render();
			}
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

void GameObject::RemoveChild([[maybe_unused]] GameObject* child)
{
	// Look for the child in the container and remove if found
	/*
	m_vChildren.erase(std::remove_if(m_vChildren.begin(), m_vChildren.end(),
		[child](const std::unique_ptr<GameObject>& ptr) { return ptr.get() == child; }), m_vChildren.end()); */

	/*
	auto childItr = std::find(m_vChildren.begin(), m_vChildren.end(), child);

	if (childItr != m_vChildren.end())
	{
		// Found
		m_vChildren.erase(childItr);
	}
	*/
}

void GameObject::AddChild(GameObject* child)
{
	m_vChildren.emplace_back(child);
}

// After updating all gameObjects check if there are any "dead" children
void GameObject::RemoveDeadChildren()
{

	for (auto itr { m_vChildren.begin()}; itr != m_vChildren.end();)
	{
		auto child = itr->get();
		if (child->IsMarkedAsDead())
		{
			// Destroy child and all the children of this child if it has any
			itr = m_vChildren.erase(itr);
		}
		else
		{
			if (child->HasChildren())
			{
				child->RemoveDeadChildren();
			}

			itr++;
		}	
	}

}

void GameObject::DestroyChild(GameObject* child)
{
	m_vChildren.erase(std::remove_if(m_vChildren.begin(), m_vChildren.end(),
		[child](const std::unique_ptr<GameObject>& ptr) { return ptr.get() == child; }), m_vChildren.end());
}

const glm::vec3 GameObject::GetWorldPosition() const
{
	// TODO : What if the transformCP doesnt exit?
	return m_pTransformCP->GetWorldPosition();

}

bool GameObject::HasChildren() const
{

	return m_vChildren.size() > 0;

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

void GameObject::MarkAsDead()
{
	m_IsDead = true;

	if (m_pParent != nullptr)
	{
		// If it has a parent --> Indicate that his child is marked as dead
		m_pParent->IncCountDeadChildren();
	}
}

void GameObject::IncCountDeadChildren()
{
	m_TotalDeadChildren++;
}

void GameObject::SetPositionDirty()
{
	if (m_pTransformCP != nullptr)
	{
		m_pTransformCP->SetPositionDirty();
	}
}