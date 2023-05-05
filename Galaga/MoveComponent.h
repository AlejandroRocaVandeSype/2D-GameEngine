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
		Boundaries(float left, float right, float bot, float top, bool restrictMovement = false)
			: leftLimit{ left }, rightLimit{ right }, botLimit{ bot }, topLimit{ top }, isMoveRestricted { restrictMovement}
			{};

		const float leftLimit;
		const float rightLimit;
		const float botLimit;
		const float topLimit;
		bool isMoveRestricted;
	};

	// The direction of the movement will be indicated through commands (either by the Player or AI)
	MoveComponent(engine::GameObject* pOwner, float speed, const Boundaries& boundaries);
	// The GameObject will move towards the direction passed through the constructor
	MoveComponent(engine::GameObject* pOwner, float speed, const Boundaries& boundaries, const glm::vec3& direction);
	virtual ~MoveComponent() override;

	virtual void Update(const float deltaTime) override;
	virtual void ReceiveMessage(const std::string& message, const std::string& value) override;

	void Move(float deltaTime, const glm::vec3& direction);
	void ChangeDirection(const glm::vec3& newDirection);			// Only for auto Movement

	bool InsideBoundaries() const;

private :
	const float SPEED;
	const Boundaries m_Boundaries;
	glm::vec2 m_GObjectSize;
	glm::vec3 m_Direction;			// Only for auto movement
	bool m_IsInsideBoundaries;
	bool m_AutoMovement;			// Controlled movement or just automatic
	
};

#endif