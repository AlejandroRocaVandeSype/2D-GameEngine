#include "GameObject.h"
#include "SceneManager.h"
#include <iostream>

using namespace engine;

GameObject::GameObject(GameObject* pParent, const std::string& tag, glm::vec3 startPosition, glm::vec2 scale, bool keepWorldPosition)
	: m_pParent { nullptr }
	, m_IsActive{ true }
	, m_IsDead { false }
	, m_HasToRender { false }
	, m_pRenderCP{ nullptr }
	, m_Tag { tag }
	, m_KeepWorldPosition { keepWorldPosition }
{
	// All gameObjects have a transform component attach when created
	m_pTransformCP = AddComponent<TransformComponent>(this, startPosition, scale);

	SetParent(pParent, m_KeepWorldPosition);
	
}

// -----------------------------------------------------------------------------
//				*Updates the parent of the GameObject*
// pNewParent = nullptr --> Current parent will be removed (if it has one)
// This function doesn't destroy any GameObject. Only updates the scenegraph hierarchy
// -----------------------------------------------------------------------------
void GameObject::SetParent(GameObject* pNewParent, bool keepWorldPosition)
{
	m_KeepWorldPosition = keepWorldPosition;	// Does the child want to keep its world position?

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
		if (m_KeepWorldPosition)
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

// -----------------------------------------------------------------------------
//					*Remove child from parent's container*
// It doesn't delete the child from the scene
// -----------------------------------------------------------------------------
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

// -----------------------------------------------------------------------------
//					*Add child to parent's container*
// This (GameObject) is gonna be child new parent
// child will be now owned by the this GameObject
// -----------------------------------------------------------------------------
void GameObject::AddChild(GameObject* child)
{
	m_vChildren.emplace_back(child);
}

// -----------------------------------------------------------------------------
//					*Update the GameObject*
// Update all the components from the GameObject
// Do this for all his children too if he has any
// -----------------------------------------------------------------------------
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

const bool GameObject::IsMarkedAsDead() const
{
	return m_IsDead;
}

void GameObject::SetParentActivity(bool activate)
{
	m_IsParentDeactivated = activate;
}


void GameObject::Render() const
{
	if (m_IsActive)
	{
		if (m_HasToRender && m_pRenderCP != nullptr)
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

// Send a message to all components
void GameObject::SendMessage(const std::string& message, const std::string& value)
{
	// TODO: Check who sent the message to not send it him again
	for (auto& componentItr : m_vComponents)
	{
		componentItr->ReceiveMessage(message, value);
	}
}


// -----------------------------------------------------------------------------
//				*Delete all children that are marked as "dead"*
// This function is called after all GameObjects have been updated
// It will remove them from the scene
// -----------------------------------------------------------------------------
void GameObject::DeleteDeadChildren()
{
	&SceneManager::GetInstance();
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
				child->DeleteDeadChildren();
			}

			itr++;
		}	
	}

}

bool GameObject::HasChildren() const
{
	return m_vChildren.size() > 0;
}


void GameObject::DeleteChild(GameObject* child)
{
	m_vChildren.erase(std::remove_if(m_vChildren.begin(), m_vChildren.end(),
		[child](const std::unique_ptr<GameObject>& ptr) { return ptr.get() == child; }), m_vChildren.end());
}

// Remove this GameObject as parent from all his children
void GameObject::RemoveParentFromChildren()
{
	while(!m_vChildren.empty())
	{
		// Set parent will reduce the size of the children container
		m_vChildren.at(0)->SetParent(nullptr);
	}
}

const glm::vec3 GameObject::GetWorldPosition() const
{
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

const GameObject* GameObject::getParent() const
{
	if (m_IsParentDeactivated)
		return nullptr;

	return m_pParent;
}

bool GameObject::HasParent() const
{
	if (m_pParent != nullptr)
	{
		return true;
	}
	return false;
}

std::vector<engine::GameObject*> GameObject::GetChildren()
{
	std::vector<engine::GameObject*> children;
	for (size_t idx{ 0 }; idx < m_vChildren.size(); ++idx)
	{
		children.emplace_back(m_vChildren.at(idx).get());
	}

	return children;
}

void GameObject::SetIsActive(const bool isActive)
{
	m_IsActive = isActive;
	if (m_IsActive && m_pTransformCP)
	{
		// Ask for the world position so it will update its position if moved
		m_pTransformCP->GetWorldPosition();
	}
}

bool GameObject::IsActive() const
{
	return m_IsActive;
}

// -----------------------------------------------------------------------------
//				*Mark the GameObject as "dead"*
// It will delete the GameObject from the scene (Including all his children if any)
// This will only happens after the update loop is finished
// -----------------------------------------------------------------------------
void GameObject::MarkAsDead()
{
	m_IsDead = true;

}


void GameObject::SetPositionDirty()
{
	if (m_pTransformCP != nullptr && !m_KeepWorldPosition)
	{
		m_pTransformCP->SetPositionDirty();
	}
}


void GameObject::SavePreviousWorldPosition(const glm::vec3& prevWorldPos)
{
	m_PreviousWorldPosition = prevWorldPos;
}

std::string GameObject::Tag() const
{
	return m_Tag;
}

GameObject::~GameObject()
{
	std::cout << "GameObject destructor" << std::endl;

	m_pTransformCP = nullptr;
	m_pRenderCP = nullptr;

}