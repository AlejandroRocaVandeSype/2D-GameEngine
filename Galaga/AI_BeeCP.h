#ifndef GALAGA_AI_BEECP
#define GALAGA_AI_BEECP
#include <Component.h>
#include "glm/vec3.hpp"

class EnemyCP;
class MoveComponent;
namespace engine
{
	class TransformComponent;
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
		diagonalMov,
		verticalMov
	};

	void UpdateAttack(const float deltaTime);
	void UpdateMoveToFormation(const float deltaTime);

	EnemyCP* m_pEnemyCP;
	MoveComponent* m_pMoveCP;
	engine::TransformComponent* m_pTransformCP;
	AttackState m_AttackState;

	float m_ElapsedTimeMov;
	float m_MaxTimeMov;
	glm::vec3 m_Direction;
};

#endif

