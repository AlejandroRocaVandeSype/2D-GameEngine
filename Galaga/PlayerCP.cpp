#include "PlayerCP.h"
#include "GameObject.h"
#include "CollisionComponent.h"
#include "HealthComponent.h"
#include "PlayerInputCP.h"
#include "MoveComponent.h"
#include "MissileManagerCP.h"
#include "LivesUIComponent.h"
#include "PlayerScoreCP.h"
#include <iostream>


PlayerCP::PlayerCP(engine::GameObject* pOwner, unsigned int health, const unsigned int playerIdx, const glm::vec2& windowLimits)
	: Component("PlayerCP", pOwner)
	, m_PlayerIdx { playerIdx }
{
	if (pOwner != nullptr)
	{

		std::string sprFileName{ "Sprites/Player.png" };
		auto renderCP = pOwner->AddComponent<engine::RenderComponent>(pOwner, sprFileName);
		auto healthCP = pOwner->AddComponent<HealthComponent>(pOwner, health);
		healthCP->AddObserver(this);

		// MOVEMENT
		glm::vec2 playerSpeed{ 150.f, 150.f };
		MoveComponent::Boundaries playerBoundaries{ 0.f, windowLimits.x, windowLimits.y, 0.f, true };
		pOwner->AddComponent<MoveComponent>(pOwner, playerSpeed, playerBoundaries);

		// MISSILES 
		int maxMissiles{ 2 };
		glm::vec2 missileSpeed{ 450.f, 450.f };
		auto missileManagerCP = pOwner->AddComponent<MissileManagerCP>(pOwner, maxMissiles, missileSpeed, "player", "Sprites/PlayerBullet.png");

		// COLLISIONS ENABLED
		pOwner->AddComponent<engine::CollisionComponent>(pOwner, renderCP->GetTextureSize());
		//collisionCP->AddObserver(this);

		// UI
			// LIVES
		glm::vec2 UIPos = { 10.f, windowLimits.y - 50.f };
		if (m_PlayerIdx == 2)
		{
			// Lives will be displayed on the right side of the screen for Player 2
			UIPos.x = windowLimits.x - 100.f;
		}
		auto livesUICP = pOwner->AddComponent<LivesUIComponent>(pOwner, sprFileName, UIPos, health - 1);
		healthCP->AddObserver(livesUICP);

			// SCORE
		auto scoreCP = pOwner->AddComponent<PlayerScoreCP>(pOwner, m_PlayerIdx);
		missileManagerCP->AddObserverToMissiles(scoreCP);		// Missiles will notify the score when to increase score

	}
}

unsigned int PlayerCP::GetPlayerIndex()
{
	return m_PlayerIdx;
}

PlayerCP::~PlayerCP()
{

}

void PlayerCP::Update(const float)
{
	
}
void PlayerCP::ReceiveMessage(const std::string& , const std::string& )
{

}

void PlayerCP::OnNotify(engine::GameObject*, const engine::Event& event)
{
	if (event.IsSameEvent("HealthDecremented"))
	{
		
	}

	if (event.IsSameEvent("GameObjectDied"))
	{
		GetOwner()->SetIsActive(false);

		/*
		// This is only if we actually destroy the player gameObject
		auto playerInputCP = GetOwner()->GetComponent<PlayerInputCP>();
		if (playerInputCP != nullptr)
		{
			// Indicate the player died to ensure we unbind all the commands associated 
			playerInputCP->SetPlayerDied();
		}
		*/
	}
}