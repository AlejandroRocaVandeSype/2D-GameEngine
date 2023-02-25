#include "GameObject.h"


dae::GameObject::GameObject()
	: m_IsActive{ true }
	, m_HasRender { false }
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


const bool dae::GameObject::HasARender() const
{
	return m_HasRender;
}