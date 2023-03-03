#include "RenderComponent.h"
#include "Renderer.h"
#include "ResourceManager.h"
#include <iostream>
using namespace dae;


RenderComponent::RenderComponent(GameObject* pOwner)
	: Component("RenderCP", pOwner)
	, m_texture { nullptr }
{
	
}

RenderComponent::RenderComponent(GameObject* pOwner, const std::string& filename)
	: Component("RenderCP", pOwner)
{
	SetTexture(filename);
}


void RenderComponent::Update([[maybe_unused]] const float deltaTime)
{

}

void RenderComponent::ReceiveMessage([[maybe_unused]] const std::string& message, [[maybe_unused]] const std::string& value)
{

}

RenderComponent::~RenderComponent()
{
	std::cout << "RenderComponent destructor" << std::endl;
}

void RenderComponent::Render(const glm::vec3& position) const
{
	if (m_texture != nullptr)
	{
		dae::Renderer::GetInstance().RenderTexture(*m_texture, position.x, position.y);
	}
	
}

// Try to create a new texture in the resourceManager and returns it created
void RenderComponent::SetTexture(const std::string& filename)
{
	m_texture = dae::ResourceManager::GetInstance().LoadTexture(filename);
}

void RenderComponent::SetTexture(std::shared_ptr<dae::Texture2D> texture)
{
	m_texture = texture;
}



