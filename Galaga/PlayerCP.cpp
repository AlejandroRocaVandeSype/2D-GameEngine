#include "PlayerCP.h"
#include "GameObject.h"
#include "CollisionComponent.h"
#include "HealthComponent.h"
#include "PlayerInputCP.h"
#include "MoveComponent.h"
#include "MissileManagerCP.h"
#include "LivesUIComponent.h"
#include <iostream>

PlayerCP::PlayerCP(engine::GameObject* pOwner, unsigned int health, const glm::vec2& windowLimits)
	: Component("PlayerCP", pOwner)
{
	if (pOwner != nullptr)
	{
		std::string sprFileName{ "Player.png" };
		pOwner->AddComponent<engine::RenderComponent>(pOwner, sprFileName);
		pOwner->AddComponent<HealthComponent>(pOwner, health);

		// INPUT FOR PLAYER
		pOwner->AddComponent<PlayerInputCP>(pOwner);
		pOwner->GetComponent<PlayerInputCP>()->AddControllerMovement();

		// MOVEMENT
		float playerSpeed{ 150.f };
		MoveComponent::Boundaries playerBoundaries{ 0.f, windowLimits.x, windowLimits.y, 0.f, true };
		pOwner->AddComponent<MoveComponent>(pOwner, playerSpeed, playerBoundaries);

		// MISSILES 
		int maxMissiles{ 2 };
		float missileSpeed{ 300.f };
		pOwner->AddComponent<MissileManagerCP>(pOwner, maxMissiles, missileSpeed);

		// COLLISIONS ENABLED
		pOwner->AddComponent<engine::CollisionComponent>(pOwner, pOwner->GetComponent<engine::RenderComponent>()->GetTextureSize());
		pOwner->GetComponent<engine::CollisionComponent>()->AddObserver(this);

		// UI
		pOwner->AddComponent<LivesUIComponent>(pOwner, sprFileName, glm::vec2{ 10.f, windowLimits.y - 50.f}, health - 1);
		pOwner->GetComponent<HealthComponent>()->AddObserver(pOwner->GetComponent<LivesUIComponent>());
	}
}

PlayerCP::~PlayerCP()
{

}

void PlayerCP::Update([[maybe_unused]] const float deltaTime)
{
	
}
void PlayerCP::ReceiveMessage([[maybe_unused]] const std::string& message, [[maybe_unused]] const std::string& value)
{

}

void PlayerCP::OnNotify([[maybe_unused]] engine::GameObject* gameObject, const engine::Event& event)
{
	if (event.IsSameEvent("CollisionWith Enemy"))
	{
		auto healthCP = GetOwner()->GetComponent<HealthComponent>();
		if (healthCP != nullptr)
		{
			healthCP->DecrementHealth(1);
		}

		auto playerInputCP = GetOwner()->GetComponent<PlayerInputCP>();
		if (playerInputCP != nullptr)
		{
			// Indicate the player died to ensure we unbind all the commands associated 
			playerInputCP->SetPlayerDied();
		}
	}
}