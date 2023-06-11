#ifndef GALAGA_ROTATORCOMPONENT
#define GALAGA_ROTATORCOMPONENT
#include "Component.h"
#include <glm/vec3.hpp>

namespace engine 
{
	class TransformComponent;
}

// Allows the gameObject to perform a simple rotation around a circle
// Radius, center, angle and other parameters need to be provided in order to perform the rotation
class RotatorComponent final: public engine::Component
{
public:
	RotatorComponent(engine::GameObject* pOwner);
	~RotatorComponent() override;
	virtual void Update(const float deltaTime) override;
	virtual void ReceiveMessage(const std::string& message, const std::string& value) override;

	void Rotate(const float deltaTime);

	void SetDataRotation(float radius, float rotationTime, float rotationTarget, const glm::vec3 center, bool isPositiveRot,
	float startAngle = 0);

	const bool IsRotationFinish() const;

private:

	float m_Radius;
	float m_RotationTime;					// Time it will take to complete a circle rotation
	float m_RotationTarget;					// Until which value rotate
	const float FULL_ROTATION_VALUE;
	const float HALF_ROTATION_VALUE;
	float m_Angle;
	glm::vec3 m_RotationCenter;
	engine::TransformComponent* m_pTransformCP;
	bool m_IsRotationFinish;
	bool m_IsPositiveRot;					// Incrementing rotation or decrementing rotation

};

#endif