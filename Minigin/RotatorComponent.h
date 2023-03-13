#pragma once
#include "Component.h"
#include <glm/glm.hpp>

class TransformComponent;
class RotatorComponent final: public Component
{
public:
	RotatorComponent(dae::GameObject* pOwner, float radius, float rotationTime);
	~RotatorComponent();
	virtual void Update(const float deltaTime) override;
	virtual void ReceiveMessage(const std::string& message, const std::string& value) override;

private:

	const float RADIUS;
	const float ROTATION_TIME;					// Time it will take to complete a circle rotation
	const float FULL_ROTATION_VALUE;
	float m_Angle;
	glm::vec3 m_CirclePos;

	TransformComponent* m_pTransformCP{};
};

