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

	virtual void Update(const float deltaTime) override;
	virtual void ReceiveMessage(const std::string& message, const std::string& value) override;

private:
	
	enum class AttackState
	{
		breakFormation,			
		diagonalDive,
		verticalDive,
		roundSwoop
	};

	void UpdateAttack(const float deltaTime);
	void UpdateMoveToFormation(const float deltaTime);
	void UpdateDiagonalDive(const float deltaTime, const glm::vec3& currentPos, const engine::Window& window);

	EnemyCP* m_pEnemyCP;
	MoveComponent* m_pMoveCP;
	engine::TransformComponent* m_pTransformCP;
	RotatorComponent* m_pRotatorCP;
	MissileManagerCP* m_pMissileManagerCP;
	AttackState m_AttackState;

	// ATTACK STATE
	float m_ElapsedDiagonalDive;
	float m_DiagonalDiveMaxTime;
	glm::vec3 m_Direction;						// Keeps track of the direction the enemy is facing
	bool m_AtRightSide;							// Enemy at right side of the window? 

	// Shooting
	bool m_HasShoot;
	unsigned short m_AmountMissiles;			// How many missiles he will be able to shoot
	unsigned short m_MissilesShoot;				// How many missiles he has already shot
	float m_ElapsedShootTime;
	float m_WaitBetweenShoot;					// How much time wait between one missile and another

	// SWOOP ROUND 
	const float ROTATION_TIME;
	float m_RotationRadius;
};

#endif

