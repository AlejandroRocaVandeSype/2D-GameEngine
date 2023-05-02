#ifndef GAME_ENGINE_ROTATORCOMPONENT
#define GAME_ENGINE_ROTATORCOMPONENT
#include "Component.h"
#include <glm/glm.hpp>

namespace engine {
	class TransformComponent;
}

class RotatorComponent final: public engine::Component
{
public:
	RotatorComponent(engine::GameObject* pOwner, float radius, float rotationTime);
	~RotatorComponent() override;
	virtual void Update(const float deltaTime) override;
	virtual void ReceiveMessage(const std::string& message, const std::string& value) override;

private:

	const float RADIUS;
	const float ROTATION_TIME;					// Time it will take to complete a circle rotation
	const float FULL_ROTATION_VALUE;
	float m_Angle;
	glm::vec3 m_CirclePos;

	engine::TransformComponent* m_pTransformCP{};
};

#endif