#ifndef GALAGA_STARTCOMMAND
#define GALAGA_STARTCOMMAND
#include <Command.h>

class MenuState;
class StartCommand final: public Command
{
public:
	explicit StartCommand(MenuState* gameState);
	virtual ~StartCommand() override;

	void Execute(float deltaTime) override;

private:
	MenuState* m_pMenuState;

};

#endif

