#include "SkipLevelCommand.h"
#include "SceneManager.h"
#include "GameplayState.h"
#include <iostream>

SkipLevelCommand::SkipLevelCommand(GameplayState* gameplayState)
	: m_pGameplayState { gameplayState }
{

}

SkipLevelCommand::~SkipLevelCommand()
{

}

void SkipLevelCommand::Execute(float)
{

	auto& sceneManager = engine::SceneManager::GetInstance();
	if (!sceneManager.AreScenesLeft())
		return;		// No more levels to skip
	
	if (m_pGameplayState != nullptr  && m_pGameplayState->ArePlayersAlive())
	{
		// Go next stage deactivating the enemies from the current stage
		m_pGameplayState->SkipStage();
	}

}