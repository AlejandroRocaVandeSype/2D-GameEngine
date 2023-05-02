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

	explicit MoveCommand(engine::GameObject* actor, glm::vec3 direction);
	virtual ~MoveCommand() override;
	void Execute(float deltaTime) override;

private :
	glm::vec3 m_Direction;		// Normalized vector that indicates the direction of the movement
	engine::GameObject* m_Actor;
};

#endif
