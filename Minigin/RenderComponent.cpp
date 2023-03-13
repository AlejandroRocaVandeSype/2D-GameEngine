#include "RenderComponent.h"
#include "Renderer.h"
#include "ResourceManager.h"
#include "Texture2D.h"
#include "GameObject.h"
#include <iostream>
using namespace dae;


RenderComponent::RenderComponent(GameObject* pOwner)
	: Component("RenderCP", pOwner)
	, m_texture { nullptr }
	, m_Scale { glm::vec2{ 1.f, 1.f } }
	, m_TextureSize{ glm::vec2{} }
	, m_IsTextureDirty { false }
{
	SetScale();
}

RenderComponent::RenderComponent(GameObject* pOwner, const std::string& filename)
	: Component("RenderCP", pOwner)
	, m_Scale{ glm::vec2{ 1.f, 1.f } }
	, m_TextureSize{ glm::vec2{} }
	, m_IsTextureDirty{ true }
{
	SetTexture(filename);
	SetScale();
}


void RenderComponent::Update([[maybe_unused]] const float deltaTime)
{
	if (m_IsTextureDirty)
	{
		glm::ivec2 textureSize{ m_texture->GetSize() };
		// Scale the texture 
		m_TextureSize.x = textureSize.x * m_Scale.x;
		m_TextureSize.y = textureSize.y * m_Scale.y;

		m_IsTextureDirty = false;
	}
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
		dae::Renderer::GetInstance().RenderTexture(*m_texture, position.x, position.y, m_TextureSize.x, m_TextureSize.y);
	
	}
	
}

// Get the scale from the transform component and set it if the gameObject is scaled
void RenderComponent::SetScale()
{
	GameObject* pOwner{ GetOwner() };
	if (pOwner != nullptr)
	{
		TransformComponent* t = pOwner->GetComponent<TransformComponent>();

		if (t != nullptr)
		{
			glm::vec2 objectScale{ t->GetScale() };

			if (objectScale.x != 1.f || objectScale.y != 1.f)
			{
				// Object needs to be scaled
				m_Scale = objectScale;
			}
		}
	}
}

// Try to create a new texture in the resourceManager and returns it created
void RenderComponent::SetTexture(const std::string& filename)
{
	m_texture = dae::ResourceManager::GetInstance().LoadTexture(filename);

	if (m_texture != nullptr)
	{
		// New texture loaded
		m_IsTextureDirty = true;
	}

}

void RenderComponent::SetTexture(std::shared_ptr<dae::Texture2D> texture)
{
	m_texture = texture;

	if (m_texture != nullptr)
	{
		m_IsTextureDirty = true;
	}
	
}



