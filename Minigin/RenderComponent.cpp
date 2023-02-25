#include "RenderComponent.h"
#include "Renderer.h"
#include "ResourceManager.h"


RenderComponent::RenderComponent()
	: Component(ComponentType::RenderCP),
	m_needsUpdate{ true }
{

}


void RenderComponent::Update([[maybe_unused]] const float deltaTime)
{
	if (m_needsUpdate)
	{
		m_needsUpdate = false;
	}
}

void RenderComponent::Render(const glm::vec3& position) const
{
	if (m_texture != nullptr)
	{
		dae::Renderer::GetInstance().RenderTexture(*m_texture, position.x, position.y);
	}
	
}


void RenderComponent::SetTexture(const std::string& filename)
{
	m_texture = dae::ResourceManager::GetInstance().LoadTexture(filename);
}


