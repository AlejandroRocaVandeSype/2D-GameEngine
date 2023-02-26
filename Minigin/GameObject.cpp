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
	m_pTransformCP = AddComponent<TransformComponent>();
}

GameObject::~GameObject()
{
	//std::cout << "entro" << std::endl;
	for (auto& componentItr : m_vComponents)
	{
		delete componentItr;
	}
	m_vComponents.clear();
}

void GameObject::Update([[maybe_unused]] const float deltaTime)
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

void GameObject::RemoveComponent(Component::ComponentType type)
{
	
	for (auto componentItr = m_vComponents.begin(); componentItr != m_vComponents.end(); ++componentItr)
	{
		if ((*componentItr) != nullptr && (*componentItr)->Type() == type)
		{
			delete* componentItr;
			m_vComponents.erase(componentItr);

			if (type == Component::RenderCP)
			{
				// In case the render was delete it, we dont want to render anymore
				m_HasToRender = false;
			}

			break;
		}
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