#include "AI_GalagaCP.h"
#include "../General/MoveComponent.h"
#include "FormationCP.h"
#include "../General/RotatorComponent.h"
#include "../General/MissileManagerCP.h"
#include "EnemyCP.h"
#include "Scene.h"
#include "../../Galaga_Strings.h"
#include "AnimationCP.h"
#include "Servicealocator.h"
#include "../Sound/SoundIDs.h"
#include <glm/gtc/constants.hpp>
#include <glm/glm.hpp>

AI_GalagaCP::AI_GalagaCP(engine::GameObject* pOwner)
	:Component("AI_GalagaCP", pOwner)
	, m_AttackState{ AttackState::breakFormation }, m_BombingRunState{ BombinRunState::divingLoop }
	, m_TractorBeamState{ TractorBeamState::moveIntoPosition }, m_DoTractorBeam{ true }, m_pGalagaTransfCP{ nullptr }
	, m_pMoveCP{ nullptr }, m_pRotatorCP{ nullptr }, ROTATION_TIME{ 1.5f }, m_RotationRadius{ 30.f }, m_DoRotateLeft{ false }
	, m_pEnemyCP{ nullptr }, m_TractorBeamPos{ glm::vec2{0.f, 0.f} }, m_Direction{ 0.f, 0.f, 0.f }
	, MAX_TRACTORBEAM_TIME{ 3.5f }, m_ElapsedTime{ 0.f }, m_pTractorBeam{ nullptr }
{

	if (pOwner != nullptr)
	{
		m_pMoveCP = pOwner->GetComponent<MoveComponent>();
		m_pGalagaTransfCP = pOwner->GetComponent<engine::TransformComponent>();
		m_pRotatorCP = pOwner->GetComponent<RotatorComponent>();
		m_pEnemyCP = pOwner->GetComponent<EnemyCP>();
	}

	// Randomly decide on which side to rotate
	m_DoRotateLeft = std::rand() % 2;

	auto window = engine::SceneManager::GetInstance().GetSceneWindow();
	m_TractorBeamPos.y = window.height - 250.f;

	if (m_pGalagaTransfCP != nullptr)
	{
		auto tractorBeamPos = m_pGalagaTransfCP->GetLocalPosition();
		tractorBeamPos.y += 30.f;
		tractorBeamPos.x -= 32.f;
		// TractorBeam object with the sprite
		m_pTractorBeam = new engine::GameObject(GetOwner(), STR_GALAGA, tractorBeamPos, glm::vec2{2.f, 2.f}, true);
		m_pTractorBeam->AddComponent<engine::RenderComponent>(m_pTractorBeam, TRACTOR_BREAM_SPRITE);

		// Animation data
		float frameRate{ 1.f / 10.f };
		int totalCols{ 3 };
		int totalFrames{ 18 };
		int startFrame{ 0 };
		int limitFrame{ 16 };
		int frameInc{ 1 };
		engine::AnimationCP::AnimationMode mode = engine::AnimationCP::AnimationMode::normalAndBack;
		m_pTractorBeam->AddComponent<engine::AnimationCP>(m_pTractorBeam, totalCols, totalFrames, frameRate, frameInc, limitFrame
			, startFrame, mode);
		m_pTractorBeam->SetIsActive(false);
	}

}

AI_GalagaCP::~AI_GalagaCP()
{

}

void AI_GalagaCP::InitData(const engine::Window window)
{
	if (m_pRotatorCP != nullptr)
	{
		bool positiveRot = false;
		auto rotationCenter = m_pGalagaTransfCP->GetLocalPosition();
		float rotationAngle{}, targetRotation{};
		if (m_DoRotateLeft)
		{
			rotationCenter.x -= m_RotationRadius;
			rotationAngle = 0.f;
			targetRotation = -glm::pi<float>();
		}
		else
		{
			rotationCenter.x += m_RotationRadius;
			rotationAngle = glm::pi<float>();
			targetRotation = glm::two_pi<float>();		// Half rotation
			positiveRot = true;
		}

		m_pRotatorCP->SetDataRotation(m_RotationRadius, ROTATION_TIME, targetRotation,
			rotationCenter, positiveRot, rotationAngle);
	}

	if (m_DoTractorBeam)
	{
		// Determine on which x Position will need to go to do the Tractor beam
		float maxXpos = window.width - 150.f;
		float minXpos = 80.f;
		 m_TractorBeamPos.x = float((std::rand() % int(maxXpos)) + minXpos);

	}
	
	// Start galaga dive sound
	auto& soundSystem = engine::Servicealocator::Get_Sound_System();
	soundSystem.PlaySound(short(Sounds::galagaDive));

	m_AttackState = AttackState::startLoop;
}

void AI_GalagaCP::Update(const float deltaTime)
{
	if (m_pMoveCP != nullptr && m_pGalagaTransfCP != nullptr)
	{
		if (m_pEnemyCP->GetCurrentState() == EnemyCP::ENEMY_STATE::attack)
		{
			auto currentPos = m_pGalagaTransfCP->GetWorldPosition();
			auto window = engine::SceneManager::GetInstance().GetSceneWindow();
			switch (m_AttackState)
			{
			case AI_GalagaCP::AttackState::breakFormation:
				InitData(window);
				break;
			case AI_GalagaCP::AttackState::startLoop:
				LoopAndDive(deltaTime);
				break;
			case AI_GalagaCP::AttackState::doAttack:
				UpdateAttack(deltaTime, currentPos);
				break;
			case AI_GalagaCP::AttackState::finishAttack:
				LeaveLevel(deltaTime, currentPos.y, window);
				break;
			}
		}
	}
}

void AI_GalagaCP::LoopAndDive(const float deltaTime)
{
	if (m_pRotatorCP != nullptr)
	{
		m_pRotatorCP->Rotate(deltaTime);
		if (m_pRotatorCP->IsRotationFinish())
		{
			m_AttackState = AttackState::doAttack;
			if (m_pEnemyCP != nullptr)
			{
				m_pEnemyCP->CalculateMissileDirection();
			}			
		}
	}
	else
	{
		// If no rotator just go directly to attack behaviour
		AI_GalagaCP::AttackState::doAttack;
	}
}

void AI_GalagaCP::UpdateAttack(const float deltaTime, const glm::vec3& currentPos)
{
	if (m_DoTractorBeam)
	{
		// TRACTOR BEAM BEHAVIOUR
		switch (m_TractorBeamState)
		{
		case AI_GalagaCP::TractorBeamState::moveIntoPosition:
			moveIntoPosition(deltaTime, currentPos);
			break;
		case AI_GalagaCP::TractorBeamState::tractorBeam:
			UpdateTractorBeam(deltaTime);
			break;
		}
	}
	else
	{
		// BOMBING RUN BEHAVIOUR
		switch (m_BombingRunState)
		{
		case AI_GalagaCP::BombinRunState::moveToLoopPoint:
			break;
		case AI_GalagaCP::BombinRunState::divingLoop:
			break;
		}
	}
}

void AI_GalagaCP::moveIntoPosition(const float deltaTime, const glm::vec3& currentPos)
{

	glm::vec3 targetPos{ m_TractorBeamPos.x, m_TractorBeamPos.y, 0.f };
	// Normalized Vector from the enemy position to the target to get the direction
	glm::vec3 direction{ glm::normalize(targetPos - currentPos) };

	float distance{ glm::distance(targetPos, currentPos) };
	if (distance > 2.f)
	{
		m_pMoveCP->Move(deltaTime, direction);
	}
	else
	{
		// In position -> Start tractor Beam
		m_TractorBeamState = TractorBeamState::tractorBeam;
		// Start tractor beam sound 
		auto& soundSystem = engine::Servicealocator::Get_Sound_System();
		soundSystem.PlaySound(short(Sounds::tractorBeam));
	}
	
}

void AI_GalagaCP::UpdateTractorBeam(const float deltaTime)
{
	m_ElapsedTime += deltaTime;
	if (m_ElapsedTime > MAX_TRACTORBEAM_TIME)
	{
		m_ElapsedTime = 0;
		m_TractorBeamState = TractorBeamState::moveIntoPosition;
		m_AttackState = AttackState::finishAttack;

		m_pTractorBeam->SetIsActive(false);
		return;
	}

	m_pGalagaTransfCP->SetPositionDirty();
	if (m_pTractorBeam != nullptr)
	{
		// Tractor beam animation
		m_pTractorBeam->SetIsActive(true);
		m_pTractorBeam->GetComponent<engine::TransformComponent>()->SetPositionDirty();
	}
	
}

void AI_GalagaCP::LeaveLevel(const float deltaTime, float galagaYPos, const engine::Window& window)
{
	if (galagaYPos < window.height)
	{
		// Move downwards outside the level
		m_pMoveCP->Move(deltaTime, {0.f, 1.f, 0.f});
	}
	else
	{
		// Set its position on top outside the level boundaries
		float formationXPos = m_pEnemyCP->GetFormationPos().x;
		float halfWindow{ window.width / 2 };
		if (formationXPos > halfWindow)
		{
			// Reset at the top right 
			m_pGalagaTransfCP->SetLocalPosition(glm::vec3{ halfWindow + 50.f, -150.f, 0.f });
		}
		else
		{
			// Reset at the top left 
			m_pGalagaTransfCP->SetLocalPosition(glm::vec3{ halfWindow - 50.f, -150.f, 0.f });
		}

		// Go back to his formation pos
		m_pEnemyCP->ChangeCurrentState(EnemyCP::ENEMY_STATE::moveToFormation);
		m_AttackState = AttackState::breakFormation;
		m_BombingRunState = BombinRunState::moveToLoopPoint;
		m_TractorBeamState = TractorBeamState::moveIntoPosition;
	}
}

void AI_GalagaCP::ReceiveMessage(const std::string& message, const std::string& value)
{
	if (message == "RemoveCP")
	{
		if (value == "TransformCP")
		{
			m_pGalagaTransfCP = nullptr;
		}
		if (value == "MoveCP")
		{
			m_pMoveCP = nullptr;
		}
		if (value == "RotatorCP")
		{
			m_pRotatorCP = nullptr;
		}
	}
}

void AI_GalagaCP::Reset()
{
	m_AttackState= AttackState::breakFormation; 
	m_BombingRunState = BombinRunState::divingLoop;
	m_TractorBeamState = TractorBeamState::moveIntoPosition;
	m_DoTractorBeam = true;
	m_DoRotateLeft= false;
	m_Direction = glm::vec3{ 0.f, 0.f, 0.f };
	m_ElapsedTime =  0.f;

	m_pTractorBeam->SetIsActive(false);
	ChangeSprite(GALAGA_SPRITE_V1);

	// Randomly decide on which side to rotate
	m_DoRotateLeft = std::rand() % 2;
}

void AI_GalagaCP::ChangeSprite(const std::string& spritePath)
{
	auto render = GetOwner()->GetComponent<engine::RenderComponent>();

	if (render != nullptr)
	{
		// Change sprite to the being hit version
		render->SetTexture(spritePath);
	}
}