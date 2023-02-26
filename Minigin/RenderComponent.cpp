#include "RenderComponent.h"
#include "Renderer.h"
#include "ResourceManager.h"

using namespace dae;

RenderComponent::RenderComponent()
	: Component(ComponentType::RenderCP)
{}


void RenderComponent::Update([[maybe_unused]] const float deltaTime)
{

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

void RenderComponent::SetTexture(std::shared_ptr<dae::Texture2D> texture)
{
	m_texture = texture;
}



