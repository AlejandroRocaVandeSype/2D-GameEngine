#ifndef GALAGA_AI_BEECP
#define GALAGA_AI_BEECP
#include <Component.h>
#include "glm/vec3.hpp"

class EnemyCP;
class MoveComponent;
class RotatorComponent;
class MissileManagerCP;
namespace engine
{
	class TransformComponent;
	struct Window;
}

class AI_BeeCP final: public engine::Component
{
public:

	AI_BeeCP(engine::GameObject* pOwner);
	virtual ~AI_BeeCP() override;

	void Update(const float deltaTime) override;
	void ReceiveMessage(const std::string& message, const std::string& value) override;

	void Reset();

private:
	
	enum class AttackState
	{
		breakFormation,				// Break the formation and set all data needed for the Attack state
		loop,						// Do a loop before diving
		diagonalDive,			
		verticalDive,
		roundSwoop					// Loop backwards to go back to the formation pos
	};

	void InitAttackData(const glm::vec3& currentPos, const engine::Window& window);
	void InitLoopData();
	void UpdateAttack(const float deltaTime);
	void UpdateDiagonalDive(const float deltaTime, const glm::vec3& currentPos, const engine::Window& window);
	void UpdateVerticalDive(const float deltaTime, const glm::vec3& currentPos, const engine::Window& window);
	void UpdateBreakLoop(const float deltaTime);

	EnemyCP* m_pEnemyCP;
	MoveComponent* m_pMoveCP;
	engine::TransformComponent* m_pTransformCP;
	RotatorComponent* m_pRotatorCP;
	AttackState m_AttackState;

	// ATTACK STATE
	float m_ElapsedDiagonalDive;
	float m_DiagonalDiveMaxTime;
	bool m_AtRightSide;							// Enemy at right side of the window? 
	glm::vec3 m_Direction;						// Keeps track of the direction the enemy is facing
	
	// For Loops
	float m_RotationTime;
	float m_RotationRadius;
};

#endif

