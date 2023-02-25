#include "RenderComponent.h"
#include "Renderer.h"
#include "ResourceManager.h"
#include <iostream>

RenderComponent::RenderComponent()
	: Component(ComponentType::RenderCP)
{

}



void RenderComponent::Update([[maybe_unused]] const float deltaTime)
{

}

void RenderComponent::Render(const glm::vec3& position) const
{
	dae::Renderer::GetInstance().RenderTexture(*m_texture, position.x, position.y);

}


void RenderComponent::SetTexture(const std::string& filename)
{
	m_texture = dae::ResourceManager::GetInstance().LoadTexture(filename);
}

