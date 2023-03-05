#pragma once
#include "Component.h"
#include <glm/glm.hpp>

class TransformComponent;
class RotatorComponent final: public Component
{
public:
	RotatorComponent(dae::GameObject* pOwner);
	~RotatorComponent();
	virtual void Update(const float deltaTime) override;
	virtual void ReceiveMessage(const std::string& message, const std::string& value) override;

private:

	const float m_Radius;
	const float m_RotationTime;			// Time it will take to complete a circle rotation
	float m_Angle;
	glm::vec3 m_PosOnCircle;
	glm::vec3 m_Center;

	TransformComponent* m_pTransformCP{};
	TransformComponent* m_pParentTransformCP{};
};

