#ifndef KILLCOMMAND
#define KILLCOMMAND

#include "Command.h"

class KillCommand final : public Command
{
public:
	explicit KillCommand(dae::GameObject* actor);
	virtual ~KillCommand() override;

	void Execute(float deltaTime) override;

private:
	dae::GameObject* m_Actor;


};

#endif
