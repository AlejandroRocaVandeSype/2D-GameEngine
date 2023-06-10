#ifndef GALAGA_AI_GALAGACP
#define GALAGA_AI_GALAGACP

#include "Component.h"
#include "glm/vec3.hpp"
#include "glm/vec2.hpp"

class MoveComponent;
class RotatorComponent;
class MissileManagerCP;
class EnemyCP;
namespace engine
{
	class TransformComponent;
	struct Window;
}
class AI_GalagaCP final : public engine::Component
{
public:
	AI_GalagaCP(engine::GameObject* pOwner);
	virtual ~AI_GalagaCP() override;

	virtual void Update(const float deltaTime) override;
	virtual void ReceiveMessage(const std::string& message, const std::string& value) override;

	void Reset();

	void ChangeSprite();

private:

	enum class AttackState
	{
		breakFormation,
		startLoop,
		doAttack,
		finishAttack
	};
	enum class BombinRunState
	{
		moveToLoopPoint,
		divingLoop
	};
	enum class TractorBeamState
	{
		moveIntoPosition,
		tractorBeam
	};

	void InitData(const engine::Window window);
	void LoopAndDive(const float deltaTime);
	void UpdateAttack(const float deltaTime, const glm::vec3& currentPos);
	void LeaveLevel(const float deltaTime, float galagaYPos, const engine::Window& window);

	// Tractor beam behaviour
	void moveIntoPosition(const float deltaTime, const glm::vec3& currentPos);
	void UpdateTractorBeam(const float deltaTime);

	// Bombing run behaviour
	AttackState m_AttackState;
	BombinRunState m_BombingRunState;
	TractorBeamState m_TractorBeamState;
	bool m_DoTractorBeam;							// To know which behavior do

	MoveComponent* m_pMoveCP;
	engine::TransformComponent* m_pGalagaTransfCP;
	RotatorComponent* m_pRotatorCP;
	EnemyCP* m_pEnemyCP;
	bool m_DoRotateLeft;							// On which direction to rotate when breaking formation

	// LOOP data
	const float ROTATION_TIME;
	float m_RotationRadius;

	glm::vec2 m_TractorBeamPos;
	glm::vec3 m_Direction;

	const float MAX_TRACTORBEAM_TIME; 
	float m_ElapsedTime;

	engine::GameObject* m_pTractorBeam;

};
#endif // DEBUG
