#ifndef MOVE_COMPONENT
#define MOVE_COMPONENT

#include "Component.h"
#include <vector>
#include <glm/glm.hpp>

class MoveComponent final : public engine::Component
{
public:

	// Simple struct to specify the boundaries for the movement
	struct Boundaries
	{
		Boundaries(float left, float right, float bot, float top)
			: leftLimit{ left }, rightLimit{ right }, botLimit{ bot }, topLimit{ top }
			{};

		float leftLimit;
		float rightLimit;
		float botLimit;
		float topLimit;
	};

	MoveComponent(engine::GameObject* pOwner, float speed, const Boundaries& boundaries);
	virtual ~MoveComponent() override;

	virtual void Update(const float deltaTime) override;
	virtual void ReceiveMessage(const std::string& message, const std::string& value) override;

	void Move(float deltaTime, glm::vec3& direction);

private :
	const float m_Speed;
	const Boundaries m_Boundaries;
	glm::vec2 m_GObjectSize;
};

#endif