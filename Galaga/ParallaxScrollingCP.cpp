#include "ParallaxScrollingCP.h"
#include "GameObject.h"
#include "TransformComponent.h"
#include <iostream>
ParallaxScrollingCP::ParallaxScrollingCP(engine::GameObject* pOwner)
	:Component("parallaxScrollingCP", pOwner)
	, m_TextureSize{ 0.f, 0.f}
	, SPEED{ 250.f}
	, m_Direction { 0.f, 1.f, 0.f}, m_IsScrollingActive { false }
{
	m_pTransformCP = pOwner->GetComponent<engine::TransformComponent>();
	engine::RenderComponent* pRenderCP = pOwner->GetComponent<engine::RenderComponent>();

	if (pRenderCP != nullptr)
	{
		m_TextureSize = pRenderCP->GetTextureSize();
	}
}

ParallaxScrollingCP::~ParallaxScrollingCP()
{
	std::cout << "ParallaxScrolling destructor" << std::endl;
}

void ParallaxScrollingCP::Update(const float deltaTime)
{
	if (m_pTransformCP != nullptr && m_IsScrollingActive)
	{
		auto position = m_pTransformCP->GetWorldPosition();

		if (position.y > m_TextureSize.y)
		{
			// Place the texture at the top again to give the scrolling effect
			position.y = 10 - m_TextureSize.y;
		}
		else
		{
			position += m_Direction * SPEED * deltaTime;
		}
		m_pTransformCP->SetLocalPosition(position);
	}
}

void ParallaxScrollingCP::ActivateScrolling()
{
	m_IsScrollingActive = true;
}

void ParallaxScrollingCP::ReceiveMessage( const std::string& message, const std::string& value)
{
	if (message == "RemoveCP")
	{
		if (value == "TransformCP")
		{
			m_pTransformCP = nullptr;
		}
	}
}