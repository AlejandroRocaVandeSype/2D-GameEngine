#ifndef MOVE_COMMAND
#define MOVE_COMMAND

#include "Command.h"
#include <glm/glm.hpp>

/* MOVEMENT COMMAND : 
	MOVE THE ACTOR IN THE DESIRED DIRECTION AND SPEED
*/
class MoveCommand final : public Command
{
public:
	explicit MoveCommand(dae::GameObject* actor, glm::vec3 direction, float speed);
	virtual ~MoveCommand() override;
	void Execute(float deltaTime) override;


private :
	float m_Speed;
	glm::vec3 m_Direction;		// Normalized vector that indicates the direction of the movement
};

#endif
