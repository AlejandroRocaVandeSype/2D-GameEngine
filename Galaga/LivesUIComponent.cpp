#include "LivesUIComponent.h"
#include "GameObject.h"
#include "Event.h"
#include "RenderComponent.h"

LivesUIComponent::LivesUIComponent(engine::GameObject* pOwner, const std::string& spriteFileName, glm::vec2 livesPos
									, unsigned int amountLives)
	:Component("LivesUICP", pOwner), Observer()
{
	if (amountLives > 0)
	{
		// CREATE THE UI LIVES GAMEOBJECTS

		// THe pOwner will own this gameObjects not the Component
		engine::GameObject* pUILive1 = new engine::GameObject(pOwner, glm::vec3{ livesPos.x, livesPos.y, 0.f }, glm::vec2{ 1.5f, 1.5f });
		pUILive1->AddComponent<engine::RenderComponent>(pUILive1, spriteFileName);
		m_vUILives.push_back(pUILive1);

		auto renderCP{ pUILive1->GetComponent<engine::RenderComponent>() };
		glm::vec2 textureSize;
		textureSize = renderCP->GetTextureSize();			// To correctly display the lives 
		
		// Create the remaining lives objects
		for (size_t livesCount{ 1 }; livesCount < amountLives; ++livesCount)
		{
			livesPos.x += textureSize.x + 5.f;

			engine::GameObject* pUILive = new engine::GameObject(pOwner, glm::vec3{ livesPos.x, livesPos.y, 0.f }, glm::vec2{ 1.5f, 1.5f });
			pUILive->AddComponent<engine::RenderComponent>(pUILive, spriteFileName);
			m_vUILives.push_back(pUILive);

		}
	}

}


LivesUIComponent::~LivesUIComponent()
{
	m_vUILives.clear();
}

void LivesUIComponent::Update([[maybe_unused]] const float deltaTime)
{

}

void LivesUIComponent::ReceiveMessage([[maybe_unused]] const std::string& message, [[maybe_unused]] const std::string& value)
{

}

void LivesUIComponent::OnNotify([[maybe_unused]] engine::GameObject* gameObject, const Event& event)
{
	if (event.IsSameEvent("HealthDecremented"))
	{
		if (!m_vUILives.empty())
		{
			// The owner will be in charge of destroying the gameObject
			m_vUILives.back()->MarkAsDead();
			m_vUILives.pop_back();
			
		}
		
	}
}