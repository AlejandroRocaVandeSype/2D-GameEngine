#include "StartCommand.h"
#include "MenuState.h"
#include <iostream>

StartCommand::StartCommand(MenuState* gameState)
	: Command()
	, m_pMenuState { gameState }
{

}

StartCommand::~StartCommand()
{
	std::cout << "Start Command destructor" << std::endl;
}

void StartCommand::Execute(float )
{
	if (m_pMenuState != nullptr)
	{
		m_pMenuState->StartGame();
	}
}
