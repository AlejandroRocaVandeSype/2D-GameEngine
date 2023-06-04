#ifndef GALAGA_MENUSELECTIONCOMMAND
#define GALAGA_MENUSELECTIONCOMMAND

#include <Command.h>

class MenuSelectionCP;
class MenuSelectionCommand final : public Command
{
public:
	explicit MenuSelectionCommand( MenuSelectionCP* pMenuSelectionCP, int menuAction);
	virtual ~MenuSelectionCommand() override;
	void Execute(float deltaTime) override;

private:
	MenuSelectionCP* m_pMenuSelection;
	int m_MenuAction;				// Indicates wheter it should go to the next Option (+), the previous one (-)
									// or select the current option (0)
};

#endif


