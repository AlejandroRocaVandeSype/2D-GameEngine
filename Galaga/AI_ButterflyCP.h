#ifndef GALAGA_AI_BUTTERFLYCP
#define GALAGA_AI_BUTTERFLYCP
#include "Component.h"
#include "glm/vec3.hpp"

class EnemyCP;
class MoveComponent;
class MissileManagerCP;
namespace engine
{
	class TransformComponent;
	struct Window;
}
class AI_ButterflyCP final : public engine::Component
{

public:

	AI_ButterflyCP(engine::GameObject* pOwner);
	virtual ~AI_ButterflyCP() override;

	virtual void Update(const float deltaTime) override;
	virtual void ReceiveMessage(const std::string& message, const std::string& value) override;

	void Reset();

private:

	enum class AttackState
	{
		breakFormation,
		diagonalDive,
		zigZagSteer
	
	};

	void InitAttackData(const glm::vec3& currentPos, const engine::Window& window);
	void UpdateAttack(const float deltaTime);
	void UpdateZigZagSteer(const float deltaTime, const glm::vec3& currentPos, const engine::Window& window);
	void UpdateDiagonalDive(const float deltaTime, const glm::vec3& currentPos, const engine::Window& window);

	EnemyCP* m_pEnemyCP;
	MoveComponent* m_pMoveCP;
	engine::TransformComponent* m_pButterflyTransfCP;
	AttackState m_AttackState;


	// ATTACK STATE
	float m_ElapsedSec;
	float m_DiagonalDiveMaxTime;
	bool m_AtRightSide;							// Enemy at right side of the window? 
	glm::vec3 m_Direction;						// Keeps track of the direction the enemy is facing

	float m_MaxSteeringTime;					// For the steer right and left


};

#endif

