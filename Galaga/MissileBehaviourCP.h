#ifndef GALAGA_MISSILE_BEHAVIOUR
#define GALAGA_MISSILE_BEHAVIOUR
#include "Component.h"
#include "Observer.h"

class MoveComponent;
class MissileBehaviourCP final : public engine::Component, public engine::Observer
{
public:
	MissileBehaviourCP(engine::GameObject* pOwner);							
	virtual ~MissileBehaviourCP() override;

	virtual void Update(const float deltaTime) override;
	virtual void ReceiveMessage(const std::string& message, const std::string& value) override;

	void OnNotify(engine::GameObject* gameObject, const engine::Event& event) override;

private:
	MoveComponent* m_pMoveComponent;
};

#endif
