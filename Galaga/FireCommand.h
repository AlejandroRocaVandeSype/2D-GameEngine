#ifndef GALAGA_FIRECOMMAND
#define GALAGA_FIRECOMMAND

#include <Command.h>
#include <glm/glm.hpp>

class FireCommand final : public Command
{
public:
	explicit FireCommand(engine::GameObject* actor, glm::vec3 direction);
	virtual ~FireCommand() override;
	void Execute(float deltaTime) override;

private:

	glm::vec3 m_Direction;
	engine::GameObject* m_Actor;
};

#endif
