#include "MissileManagerCP.h"
#include "RenderComponent.h"
#include "HealthComponent.h"
#include "MissileCP.h"
#include "TransformComponent.h"
#include "SceneManager.h"
#include "MoveComponent.h"
#include "CollisionComponent.h"
#include "Servicealocator.h"
#include "../Sound/SoundIDs.h"

MissileManagerCP::MissileManagerCP(engine::GameObject* pOwner, int maxMissiles, const glm::vec2& missileSpeed,
	const std::string& missileOwner, const std::string& texturePath)
	:Component("MissileManagerCP", pOwner)
	, SPEED{ missileSpeed }
	, MAX_MISSILES{ maxMissiles }
	, m_MissileOwner { missileOwner }
{

	auto& SceneManager = engine::SceneManager::GetInstance();
	auto window = SceneManager.GetSceneWindow();
	// Position will be updated when missiles are fired
	glm::vec3 missileStartPos{ glm::vec3{} };		

	MoveComponent::Boundaries missileBoundaries{ 0.f, window.width , window.height, 0.f, false };


	std::string missileTag{ missileOwner + "Missile" };
	bool isPlayer{ false };
	if (missileOwner == "player")
	{
		isPlayer = true;
	}

	// Create all the missiles already but set them as inactives until fired
	// Missiles will be activated/deactivated when fire instead of creating it them all the time
	for (int missileIdx{ 0 }; missileIdx < maxMissiles; ++missileIdx)
	{
		auto go_Missile = std::make_shared<engine::GameObject>(nullptr, missileTag, missileStartPos, glm::vec2{2.f, 2.f});
		auto renderCP = go_Missile->AddComponent<engine::RenderComponent>(go_Missile.get(), texturePath);
		go_Missile->AddComponent<MoveComponent>(go_Missile.get(), SPEED, missileBoundaries, glm::vec3{0.f, -1.f, 0.f});
		auto missileHealthCP = go_Missile->AddComponent<HealthComponent>(go_Missile.get(), 1);
		auto missileCP = go_Missile->AddComponent<MissileCP>(go_Missile.get(), isPlayer);
		auto collisionCP = go_Missile->AddComponent<engine::CollisionComponent>(go_Missile.get(), renderCP->GetTextureSize());
		collisionCP->AddObserver(missileCP);
		missileHealthCP->AddObserver(missileCP);

		// Missiles wont show until they are fired
		go_Missile->SetIsActive(false);			

		// The scene and the Missile Manager will share ownership of the missiles
		m_vMissiles.push_back(go_Missile);
		SceneManager.AddToActiveScene(go_Missile);
	}

}

MissileManagerCP::~MissileManagerCP()
{
	// Needed because sharing ownership with missiles
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
			missile->GetComponent<HealthComponent>()->ResetHealth(1);
			missile->SetIsActive(true);

			if (m_MissileOwner == "player")
			{
				auto& soundSystem = engine::Servicealocator::Get_Sound_System();
				soundSystem.PlaySound(short(Sounds::playerFire));
			}			
			break;
		}
	}

}

void MissileManagerCP::AddObserverToMissiles(engine::Observer* pObserver)
{
	for (auto& missile : m_vMissiles)
	{
		auto missileCP = missile->GetComponent<MissileCP>();
		if (missileCP)
			missileCP->AddObserver(pObserver);
	}
}

void MissileManagerCP::Update(const float )
{

}

void MissileManagerCP::ReceiveMessage( const std::string& ,  const std::string& )
{

}