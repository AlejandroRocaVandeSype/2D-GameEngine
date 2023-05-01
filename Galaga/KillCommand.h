#ifndef KILLCOMMAND
#define KILLCOMMAND

#include "Command.h"

class KillCommand final : public Command
{
public:
	explicit KillCommand(engine::GameObject* actor);
	virtual ~KillCommand() override;

	void Execute(float deltaTime) override;

private:
	engine::GameObject* m_Actor;


};

#endif
