#include "GameObject.h"


dae::GameObject::GameObject()
	: m_IsActive{ true }
	, m_HasToRender { false }
	, m_pRenderCP{ nullptr }
{
	// All gameObjects have a transform component attach when created
	m_pTransformCP = AddComponent<TransformComponent>();
}

dae::GameObject::~GameObject()
{
	for (auto& componentItr : m_vComponents)
	{
		delete componentItr;
	}
	m_vComponents.clear();
}

void dae::GameObject::Update([[maybe_unused]] const float deltaTime)
{
	for (auto& componentItr : m_vComponents)
	{
		componentItr->Update(deltaTime);
	}
}

void dae::GameObject::Render() const
{
	if (HasARender() && m_pRenderCP != nullptr && m_pTransformCP != nullptr)
	{
		m_pRenderCP->Render(m_pTransformCP->GetPosition());
	}
	
}

void dae::GameObject::RemoveComponent(Component::ComponentType type)
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


const bool dae::GameObject::HasARender() const
{
	return m_HasToRender;
}