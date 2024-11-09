#ifndef GALAGA_TOGGLEMUTECOMMAND
#define GALAGA_TOGGLEMUTECOMMAND

#include <Command.h>

class ToggleMuteCommand final : public Command
{
public: 
	explicit ToggleMuteCommand();
	virtual ~ToggleMuteCommand() override;
	void Execute(float deltaTime) override;

};

#endif

