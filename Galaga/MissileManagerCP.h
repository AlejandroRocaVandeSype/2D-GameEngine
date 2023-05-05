#ifndef GALAGA_MISSILE_MANAGER
#define GALAGA_MISSILE_MANAGER

#include "Component.h"
#include <glm/glm.hpp>
#include <vector>
#include "GameObject.h"

class MissileManagerCP final : public engine::Component
{
public:
	MissileManagerCP(engine::GameObject* pOwner, int maxMissiles, float missileSpeed);

	virtual ~MissileManagerCP() override;

	virtual void Update(const float deltaTime) override;
	virtual void ReceiveMessage(const std::string& message, const std::string& value) override;

	void Fire(const glm::vec3& direction);

private:
	const float SPEED;						// Speed at which all missiles will move
	const int MAX_MISSILES;					// Max amount of missiles allow to fire up  on the screen at a time

	std::vector<std::shared_ptr <engine::GameObject>> m_vMissiles;
};

#endif
