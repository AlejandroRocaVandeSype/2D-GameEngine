#include "GameCP.h"
#include "MenuState.h"

GameCP::GameCP(engine::GameObject* pOwner)
	:Component("GameCP", pOwner)
	, m_pGameState { nullptr }
{
	// First state will always be the Menu
	m_pGameState = new MenuState();
	m_pGameState->OnEnter();
}

GameCP::~GameCP()
{
	delete m_pGameState;
}

void GameCP::Update(const float deltaTime)
{

	GameState* pNewGameState = m_pGameState->GetChangeState();

	if (pNewGameState != nullptr)
	{
		// New state
		delete m_pGameState;
		m_pGameState = pNewGameState;

		pNewGameState->OnEnter();
	}

	m_pGameState->UpdateState(deltaTime);

}

void GameCP::ReceiveMessage(const std::string&, const std::string& )
{

}
