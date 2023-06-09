#ifndef GALAGA_MISSILE_BEHAVIOUR
#define GALAGA_MISSILE_BEHAVIOUR
#include "Component.h"
#include "Observer.h"
#include "Subject.h"
#include <memory>

class MoveComponent;
class HealthComponent;
class MissileCP final : public engine::Component, public engine::Observer
{
public:
	MissileCP(engine::GameObject* pOwner, bool IsPlayerMissile);							
	virtual ~MissileCP() override;

	virtual void Update(const float deltaTime) override;
	virtual void ReceiveMessage(const std::string& message, const std::string& value) override;

	void OnNotify(engine::GameObject* gameObject, const engine::Event& event) override;

	void AddObserver(engine::Observer* pObserver);

	void SetEnemyPoints(int enemyValue);
	int GetEnemyPoints() const;

private:
	MoveComponent* m_pMoveComponent;
	HealthComponent* m_pHealthCP;
	std::string m_CollisionWith;
	int m_EnemyPoints;

	engine::Event m_MissileFiredEvent;
	std::unique_ptr<engine::Subject> m_MissileSubject;
};

#endif
