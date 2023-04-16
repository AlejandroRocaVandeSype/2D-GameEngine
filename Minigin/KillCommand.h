#ifndef KILLCOMMAND
#define KILLCOMMAND

#include "Command.h"
#include "Subject.h"
#include <memory>

class KillCommand final : public Command
{
public:
	explicit KillCommand(dae::GameObject* actor);
	virtual ~KillCommand() override;

	void Execute(float deltaTime) override;

private:
	dae::GameObject* m_Actor;
	std::unique_ptr<Subject> m_ActorDiedEvent;

};

#endif
