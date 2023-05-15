#ifndef GALAGA_AI_BEECP
#define GALAGA_AI_BEECP
#include <Component.h>

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
	EnemyCP* m_pEnemyCP;
	MoveComponent* m_pMoveCP;
	engine::TransformComponent* m_pTransformCP;
};

#endif

