#include "MissileManagerCP.h"
#include "RenderComponent.h"
#include "HealthComponent.h"
#include "MissileBehaviourCP.h"
#include "TransformComponent.h"
#include "SceneManager.h"
#include "MoveComponent.h"
#include "CollisionComponent.h"
#include <iostream>

MissileManagerCP::MissileManagerCP(engine::GameObject* pOwner, int maxMissiles, float missileSpeed)
	:Component("MissileManagerCP", pOwner)
	, SPEED{ missileSpeed }
	, MAX_MISSILES{ maxMissiles }
{

	auto& SceneManager = engine::SceneManager::GetInstance();
	// Position will be updated when missiles are fired
	glm::vec3 missileStartPos{ glm::vec3{} };		

	MoveComponent::Boundaries missileBoundaries{ 0.f, 560.f , 640.f, 0.f, false };

	// Create all the missiles already but set them as inactives until fired
	// Missiles will be activated/deactivated when fire instead of creating it them all the time
	for (int missileIdx{ 0 }; missileIdx < maxMissiles; ++missileIdx)
	{
		auto go_Missile = std::make_shared<engine::GameObject>(nullptr, "PlayerMissile", missileStartPos, glm::vec2{2.f, 2.f});
		go_Missile->AddComponent<engine::RenderComponent>(go_Missile.get(), "PlayerBullet.png");
		go_Missile->AddComponent<MoveComponent>(go_Missile.get(), 300.f, missileBoundaries, glm::vec3{0.f, -1.f, 0.f});
		go_Missile->AddComponent<HealthComponent>(go_Missile.get(), 1);
		go_Missile->AddComponent<MissileBehaviourCP>(go_Missile.get());
		go_Missile->AddComponent<engine::CollisionComponent>(go_Missile.get(), go_Missile->GetComponent<engine::RenderComponent>()->GetTextureSize());
		go_Missile->GetComponent<engine::CollisionComponent>()->AddObserver(go_Missile->GetComponent<MissileBehaviourCP>());

		// Missiles wont show until they are fired
		go_Missile->SetIsActive(false);			

		// The scene and the Manager will share ownership of the missiles
		m_vMissiles.push_back(go_Missile);
		SceneManager.AddToActiveScene(go_Missile);
	}

}

MissileManagerCP::~MissileManagerCP()
{
	std::cout << "MissileManagerCP destructor" << std::endl;
	m_vMissiles.clear();
}

// Fire a missile if there is any missile available 
// If all missiles are active then no missile will be fire
// This only activates the missile and places it at the player's position
void MissileManagerCP::Fire(const glm::vec3& direction)
{
	for (auto& missile : m_vMissiles)
	{
		if (!missile->IsActive())
		{
			// Missile available to fire
			// Activate it and place it at the player current position
			missile->GetComponent<MoveComponent>()->ChangeDirection(direction);
			missile->GetComponent<engine::TransformComponent>()->SetLocalPosition(GetOwner()->GetWorldPosition());
			missile->SetIsActive(true);
			break;
		}
	}

}

void MissileManagerCP::Update([[maybe_unused]]const float deltaTime)
{

}

void MissileManagerCP::ReceiveMessage([[maybe_unused]] const std::string& message, [[maybe_unused]] const std::string& value)
{

}