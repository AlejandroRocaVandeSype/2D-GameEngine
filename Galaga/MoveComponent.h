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
		Boundaries() : leftLimit{ 0 }, rightLimit{ 0 }, botLimit{ 0 }, topLimit{ 0 }, isMoveRestricted{ false } {};
		Boundaries(float left, float right, float bot, float top, bool restrictMovement = false)
			: leftLimit{ left }, rightLimit{ right }, botLimit{ bot }, topLimit{ top }, isMoveRestricted { restrictMovement}
			{};

		float LeftLimit() { return leftLimit; };
		float RightLimit() { return rightLimit; };
		float BotLimit() { return botLimit; };
		float TopLimit() { return topLimit; };

		bool IsMoveRestricted() { return isMoveRestricted; };

	private:
		float leftLimit;
		float rightLimit;
		float botLimit;
		float topLimit;
		bool isMoveRestricted;
	};

	// The direction of the movement will be indicated through commands (either by the Player or AI)
	MoveComponent(engine::GameObject* pOwner, const glm::vec2& speed, const Boundaries& boundaries);
	// The GameObject will move towards the direction passed through the constructor
	MoveComponent(engine::GameObject* pOwner, const glm::vec2& speed, const Boundaries& boundaries, const glm::vec3& direction);
	virtual ~MoveComponent() override;

	virtual void Update(const float deltaTime) override;
	virtual void ReceiveMessage(const std::string& message, const std::string& value) override;

	void Move(float deltaTime, const glm::vec3& direction);
	void ChangeDirection(const glm::vec3& newDirection);			// Only for auto Movement
	void ChangeSpeed(const glm::vec2& newSpeed);

	bool InsideBoundaries() const;

private :
	glm::vec2 m_Speed;
	Boundaries m_Boundaries;
	glm::vec2 m_GObjectSize;
	glm::vec3 m_Direction;			// Only for auto movement (Direction is always the same)
	bool m_IsInsideBoundaries;
	bool m_AutoMovement;			// Controlled movement (Player) or just automatic (AI)
	
};

#endif