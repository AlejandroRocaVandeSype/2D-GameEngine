#ifndef GALAGA_MISSILE_MANAGER
#define GALAGA_MISSILE_MANAGER

#include "Component.h"
#include <glm/glm.hpp>
#include <vector>
#include "GameObject.h"

namespace engine
{
	class Observer;
}
class MissileManagerCP final : public engine::Component
{
public:
	MissileManagerCP(engine::GameObject* pOwner, int maxMissiles, const glm::vec2& missileSpeed, const std::string& missileOwner,
		const std::string& texturePath);

	virtual ~MissileManagerCP() override;

	virtual void Update(const float deltaTime) override;
	virtual void ReceiveMessage(const std::string& message, const std::string& value) override;

	void Fire(const glm::vec3& direction);
	void AddObserverToMissiles(engine::Observer* pObserver);

private:
	const glm::vec2& SPEED;						// Speed at which all missiles will move
	const int MAX_MISSILES;						// Max amount of missiles allow to fire up  on the screen at a time
	const std::string m_MissileOwner;
	std::vector<std::shared_ptr <engine::GameObject>> m_vMissiles;
};

#endif
