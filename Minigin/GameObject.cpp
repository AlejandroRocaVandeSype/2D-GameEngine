#include "GameObject.h"
#include "SceneManager.h"
#include <iostream>

using namespace dae;

GameObject::GameObject(GameObject* pParent, glm::vec3 startPosition, glm::vec2 scale)
	: m_pParent { nullptr }
	, m_IsActive{ true }
	, m_IsDead { false }
	, m_HasToRender { false }
	, m_pRenderCP{ nullptr }
{
	// All gameObjects have a transform component attach when created
	// When gameObject is created, WorldPos = localPos since it wont have a parent at start
	m_pTransformCP = AddComponent<TransformComponent>(this, startPosition, scale);

	SetParent(pParent);
	
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
		if (HasARender() && m_pRenderCP != nullptr)
		{
			if (m_pTransformCP != nullptr)
			{
				// Render based on the transformCP world position
				m_pRenderCP->Render(m_pTransformCP->GetWorldPosition());
			}
			else
			{
				// TransformCP was destroyed for some reason -> Use the last position saved to render
				m_pRenderCP->Render(m_PreviousWorldPosition);
			}
			
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
// we use freeChild to remove it from the children container from the parent (children will still exits)
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

	// To know if a parent lost his child and the scene needs to own the orphan child
	bool childRemoved{ false };    

	if (m_pParent != nullptr)
	{
		// This gameObject already has a parent -> Remove itself as a child (Dont destroy)
		childRemoved = m_pParent->FreeChild(this);		
	}
	m_pParent = pNewParent;

	// If we are adding a new parent to the gameObject then we need to add this as a child
	if (m_pParent != nullptr)
	{
		m_pParent->AddChild(this);
	}
	else
	{
		if (childRemoved)
		{
			// Now the scene owns this gameObject
			SceneManager::GetInstance().AddToActiveScene(this);
		}	
	}

}

const GameObject* GameObject::getParent() const
{
	return m_pParent;
}

// Look for the child in the container and remove if found (It doesnt delete it from the scene)
bool GameObject::FreeChild(GameObject* child)
{
	
	for (auto childItr{ m_vChildren.begin() }; childItr != m_vChildren.end(); ++childItr)
	{
		if (childItr->get() == child)
		{
			// Found
			childItr->release();					// Parent doesnt own the child anymore
			childItr = m_vChildren.erase(childItr);
		
			return true;
		}

	}

	return false;
}

void GameObject::AddChild(GameObject* child)
{
	m_vChildren.emplace_back(child);
}

// After updating all gameObjects check if there are any "dead" children
// If there are then destroy them 
void GameObject::RemoveDeadChildren()
{
	&dae::SceneManager::GetInstance();
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


void GameObject::RemoveParentFromChildren()
{
	for (auto childItr{ m_vChildren.begin() }; childItr != m_vChildren.end(); ++childItr)
	{
		// Problem here with itr when we remove a child from its parent
		auto child = childItr->get();
		child->SetParent(nullptr);
	}
}

const glm::vec3 GameObject::GetWorldPosition() const
{
	// TODO : What if the transformCP doesnt exit?

	if (m_pTransformCP != nullptr)
	{
		return m_pTransformCP->GetWorldPosition();
	}
	else
	{
		// Return the last world position saved
		return m_PreviousWorldPosition;
	}
	

}

bool GameObject::HasChildren() const
{
	return m_vChildren.size() > 0;
}

bool GameObject::HasParent() const
{
	if (m_pParent != nullptr)
	{
		return true;
	}
	return false;
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

}


void GameObject::SetPositionDirty()
{
	if (m_pTransformCP != nullptr)
	{
		m_pTransformCP->SetPositionDirty();
	}
}

void GameObject::SavePreviousWorldPosition(const glm::vec3& prevWorldPos)
{
	m_PreviousWorldPosition = prevWorldPos;
}