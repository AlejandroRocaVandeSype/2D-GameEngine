#ifndef GALAGA_SKIPLEVELCOMMAND
#define GALAGA_SKIPLEVELCOMMAND

#include <Command.h>
class GameplayState;
class SkipLevelCommand final : public Command
{
public:
	explicit SkipLevelCommand(GameplayState* gameplayState);
	virtual ~SkipLevelCommand() override;
	void Execute(float deltaTime) override;

private:
	GameplayState* m_pGameplayState;

};

#endif

