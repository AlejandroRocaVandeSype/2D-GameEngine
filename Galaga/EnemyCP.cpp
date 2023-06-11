#include "EnemyCP.h"
#include "Galaga_Strings.h"
#include "GameObject.h"
#include "CollisionComponent.h"
#include "HealthComponent.h"
#include "Servicealocator.h"
#include "SoundIDs.h"
#include "MoveComponent.h"
#include "RotatorComponent.h"
#include "MissileManagerCP.h"
#include "PlayerCP.h"
#include "Scene.h"
#include "AI_BeeCP.h"
#include "AI_ButterflyCP.h"
#include "AI_GalagaCP.h"
#include "MissileCP.h"
#include "AnimationCP.h"
#include <glm/glm.hpp>

EnemyCP::EnemyCP(engine::GameObject* pOwner, const std::string& enemyType, const std::string& spriteFilePath, 
	const glm::vec3 formationPos, unsigned int health)
	: Component("EnemyCP", pOwner)
	, m_FormationPos { formationPos }, m_CurrentState { ENEMY_STATE::moveToFormation }, m_EnemyType { enemyType }
	, m_pTransformCP { nullptr }, m_pMoveCP { nullptr }, m_MissileDir{ 0.f, 1.f, 0.f }, m_HasShoot{ true }
	, m_AmountMissiles{ 0 }, m_MissilesShoot{ 0 }, m_ElapsedShootTime{ 0.f }, m_WaitBetweenShoot{ 0.3f }
	, m_pMissileManagerCP{ nullptr }, m_FormationPoints { 0 }, m_DivingPoints { 0 }
{
	if (pOwner != nullptr)
	{
		auto renderCP = pOwner->AddComponent<engine::RenderComponent>(pOwner, spriteFilePath);

		// Sprite animation
		float frameRate{ 1.f / 5.f };
		int totalCols{ 16 };
		int totalFrames{ 32 };
		int frameInc{ 16 };
		int limitFrame{ frameInc };
		pOwner->AddComponent<engine::AnimationCP>(pOwner, totalCols, totalFrames, frameRate, frameInc, limitFrame);

		auto healthCP = pOwner->AddComponent<HealthComponent>(pOwner, health);
		MoveComponent::Boundaries enemyBoundaries{};  // No boundaries restriction
		m_pMoveCP = pOwner->AddComponent<MoveComponent>(pOwner, glm::vec2{ 220.f, 220.f }, enemyBoundaries);
		auto collisionCP = pOwner->AddComponent<engine::CollisionComponent>(pOwner, renderCP->GetTextureSize());
		pOwner->AddComponent<RotatorComponent>(pOwner);
		collisionCP->AddObserver(this);
		healthCP->AddObserver(this);

		int maxMissiles{ 2 };
		glm::vec2 missileSpeed{ 80.f, 400.f };
		m_pMissileManagerCP = pOwner->AddComponent<MissileManagerCP>(pOwner, maxMissiles, missileSpeed,
			"enemy", "Sprites/enemyMissile.png");

		m_pTransformCP = pOwner->GetComponent<engine::TransformComponent>();

		SetEnemyTypePoints();	
	}
}

EnemyCP::~EnemyCP()
{

}

void EnemyCP::Update(const float deltaTime)
{
	if (m_pMoveCP != nullptr && m_pTransformCP != nullptr)
	{
		switch (m_CurrentState)
		{
		case EnemyCP::ENEMY_STATE::waiting:
			// While waiting he needs to keep moving with the formation
			m_pTransformCP->SetPositionDirty();
			break;
		case EnemyCP::ENEMY_STATE::moveToFormation:
			UpdateMoveToFormation(deltaTime);
			break;
		case EnemyCP::ENEMY_STATE::attack:
			// Only handle the firing behavior
			// The enemy behavior will be updated by the AI component
			if (!m_HasShoot)
			{
				// Enemy has to shoot a missile
				FireMissile(deltaTime);
			}
			break;
		}
	}
}

// Enemy moves back to its position in the formation
void EnemyCP::UpdateMoveToFormation(const float deltaTime)
{
	auto currentPos = m_pTransformCP->GetWorldPosition();

	glm::vec3 targetPos{ GetOwner()->getParent()->GetComponent<engine::TransformComponent>()->GetWorldPosition() 
		+ m_FormationPos };
	// Normalized Vector from the enemy position to the target to get the direction
	glm::vec3 direction{ glm::normalize(targetPos - currentPos) };

	float distance{ glm::distance(targetPos, currentPos) };
	if (distance > 2.f)
	{
		m_pMoveCP->Move(deltaTime, direction);
	}
	else
	{
		// Wait until recieve orders to attack
		m_CurrentState = EnemyCP::ENEMY_STATE::waiting;
		glm::vec2 newSpeed{ 160.f, 160.f };
		if (m_EnemyType != STR_GALAGA)
		{
			// Bees and butterflies move slower in the x axis
			newSpeed.x = 80.f;
		}

		// Reduce its speed
		m_pMoveCP->ChangeSpeed(newSpeed);
	}
}

void EnemyCP::CalculateMissileDirection()
{
	// Charge 1 or 2 missiles to shoot (or zero)
	m_AmountMissiles = std::rand() % 3;
	if (m_AmountMissiles > 0)
	{
		m_HasShoot = false;			// There missiles that need to be fired
		
		auto& sceneManager = engine::SceneManager::GetInstance();
		engine::GameObject* pPlayer = sceneManager.FindGameObjectByTag(STR_PLAYER_TAG);
		if (pPlayer != nullptr)
		{
			engine::TransformComponent* pPlayerTransformCP = pPlayer->GetComponent<engine::TransformComponent>();
			if (pPlayerTransformCP != nullptr)
			{
				// Missile lways go downwards. The x direction will be determine depending where the player is
				float playerXPos{ pPlayerTransformCP->GetWorldPosition().x };
				if (playerXPos < m_pTransformCP->GetWorldPosition().x)
				{
					m_MissileDir.x = -1.f;
				}
				else
				{
					m_MissileDir.x = 1.f;
				}
				return;
			}
		}

		// No player or no valid player pos = no fire missiles
		m_HasShoot = true;
	}
}

void EnemyCP::FireMissile(const float deltaTime)
{
	m_ElapsedShootTime += deltaTime;
	if (m_ElapsedShootTime > m_WaitBetweenShoot)
	{
		if (m_pMissileManagerCP != nullptr)
		{
			m_pMissileManagerCP->Fire(m_MissileDir);
		}

		m_MissilesShoot++;
		if (m_MissilesShoot == m_AmountMissiles)
		{
			// No more missiles to shoot
			m_HasShoot = true;
			m_MissilesShoot = 0;
		}

		m_ElapsedShootTime = 0.f;
	}
}

void EnemyCP::ReceiveMessage( const std::string& message, const std::string& value)
{
	if (message == "RemoveCP")
	{
		if (value == "TransformCP")
		{
			m_pTransformCP = nullptr;
		}
		if (value == "MoveCP")
		{
			m_pMoveCP = nullptr;
		}
		if (value == "MissileManagerCP")
		{
			m_pMissileManagerCP = nullptr;
		}
	}
}

void EnemyCP::OnNotify(engine::GameObject* gameObject, const engine::Event& event)
{
	// Enemy crash into the player
	if (event.IsSameEvent("CollisionWith Player") || event.IsSameEvent("CollisionWith playerMissile"))
	{
		// Enemy loose a life
		auto enemyHealthCP = GetOwner()->GetComponent<HealthComponent>();
		if (enemyHealthCP != nullptr)
		{
			enemyHealthCP->DecrementHealth(1);
		}

		// The gameObject we collided with will also loose a life
		auto otherGOHealthCP = gameObject->GetComponent<HealthComponent>();
		if (otherGOHealthCP != nullptr)
		{
			otherGOHealthCP->DecrementHealth(1);
		}	

		if (event.IsSameEvent("CollisionWith playerMissile"))
		{
			if (enemyHealthCP != nullptr && enemyHealthCP->GetLives() <= 0)
			{
				// Set the points for killing the enemy
				int points{};
				if (m_CurrentState == ENEMY_STATE::waiting)
				{
					points = m_FormationPoints;
				}
				else
				{
					points = m_DivingPoints;
				}
				auto missileCP = gameObject->GetComponent<MissileCP>();
				if (missileCP != nullptr)
				{
					missileCP->SetEnemyPoints(points);
				}
				
			}			
		}
	}

	if (event.IsSameEvent("GameObjectDied"))
	{ 
		// Enemies will become inactive when they die (do not destroy the gameObject)
		GetOwner()->SetIsActive(false);
		auto& soundSystem = engine::Servicealocator::Get_Sound_System();
		soundSystem.PlaySound(short(Sounds::enemyDie));
	}

	if (event.IsSameEvent("HealthDecremented"))
	{
		auto galaga = GetOwner()->GetComponent<AI_GalagaCP>();
		if (galaga != nullptr) // Change sprite for galaga enemies when hit
			galaga->ChangeSprite(GALAGA_SPRITE_V2);
	}
}

void EnemyCP::Reset(const glm::vec3& startPos, const glm::vec3& formationPos)
{
	if (m_pTransformCP != nullptr)
	{
		// Reset its position to the start position for the formation
		m_pTransformCP->SetLocalPosition(startPos);
		m_FormationPos = formationPos;							// New position in the formation
		m_pMoveCP->ChangeSpeed(glm::vec2{ 220.f, 220.f });
		m_CurrentState = ENEMY_STATE::moveToFormation;
		m_HasShoot = true;
		m_ElapsedShootTime = 0.f;

		if (m_EnemyType == STR_BEE)
		{
			GetOwner()->GetComponent<HealthComponent>()->ResetHealth(1);
			auto beeAI = GetOwner()->GetComponent<AI_BeeCP>();
			if (beeAI != nullptr)
			{
				beeAI->Reset();
			}
			
		}
		else if (m_EnemyType == STR_BUTTERFLY)
		{
			GetOwner()->GetComponent<HealthComponent>()->ResetHealth(1);
			auto butterflyAI = GetOwner()->GetComponent<AI_ButterflyCP>();
			if (butterflyAI != nullptr)
			{
				butterflyAI->Reset();
			}
		}
		else if(m_EnemyType == STR_GALAGA)
		{
			GetOwner()->GetComponent<HealthComponent>()->ResetHealth(2);
			auto galagaAI = GetOwner()->GetComponent<AI_GalagaCP>();
			if (galagaAI != nullptr)
			{
				galagaAI->Reset();
			}
		}
	}
}

// Set the Points this Enemy gives depending if it is in formation
// or diving
void EnemyCP::SetEnemyTypePoints()
{

	if (m_EnemyType == STR_BEE)
	{
		m_FormationPoints = 50;
		m_DivingPoints = 100;
	}

	if (m_EnemyType == STR_GALAGA)
	{
		m_FormationPoints = 150;
		m_DivingPoints = 400;
	}

	if (m_EnemyType == STR_BUTTERFLY)
	{
		m_FormationPoints = 80;
		m_DivingPoints = 160;
	}	
}

void EnemyCP::ChangeCurrentState(EnemyCP::ENEMY_STATE newState)
{
	m_CurrentState = newState;
}

EnemyCP::ENEMY_STATE EnemyCP::GetCurrentState() const
{
	return m_CurrentState;
}

glm::vec3 EnemyCP::GetFormationPos() const
{
	return m_FormationPos;
}

const std::string& EnemyCP::GetType() const
{
	return m_EnemyType;
}