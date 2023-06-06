#include "GameplayState.h"
#include "GameObject.h"
#include "PlayerCP.h"
#include "PlayerInputCP.h"
#include "Scene.h"
#include "FormationCP.h"
#include "AI_FormationCP.h"
#include <iostream>


GameplayState::GameplayState(const std::string& gameMode)
	:m_GameMode { gameMode }
{

}

GameplayState::~GameplayState()
{
	std::cout << "Gameplay State destructor" << std::endl;
}

void GameplayState::OnEnter()
{
	if (m_GameMode == "1 PLAYER" || m_GameMode == "2 PLAYERS")
	{
		std::cout << "1 PLAYER MODE" << std::endl;

		auto& sceneManager = engine::SceneManager::GetInstance();
		auto& scene = sceneManager.GetActiveScene();

		auto& window = sceneManager.GetSceneWindow();

		InitEnemies();

		// Add the PlayerCP to Player gameObject and the input for the Gameplay
		auto pPlayer1GO = scene.FindGameObjectByTag("Player");
		if (pPlayer1GO != nullptr)
		{
			pPlayer1GO->AddComponent<PlayerCP>(pPlayer1GO, 4, 1, glm::vec2{ window.width, window.height });
			auto pPlayerInputCP = pPlayer1GO->GetComponent<PlayerInputCP>();
			if (pPlayerInputCP != nullptr && m_GameMode == "1 PLAYER")
			{
				// Player 1 can play with Keyboard and Controller
				pPlayerInputCP->GameplayControllerInput(unsigned int(0));
				pPlayerInputCP->GameplayKeyboardInput();
				return;
			}
		}		

		if (m_GameMode == "2 PLAYERS")
		{
			// Add Player 2 too ( Player 2 will use the available controller)
			glm::vec3 startPos{ (window.width / 2.f) + 50.f, window.height / 1.15f, 0.f };
			auto go_Player2 = std::make_shared<engine::GameObject>(nullptr, "Player", startPos, glm::vec2{ 2.f, 2.f });
			go_Player2->AddComponent<PlayerCP>(go_Player2.get(), 4, 2, glm::vec2{window.width, window.height});
			auto pPlayer2InputCP = go_Player2->AddComponent<PlayerInputCP>(go_Player2.get());
			pPlayer2InputCP->TwoPlayersGameplayInput();

			scene.Add(go_Player2);
	
			if (pPlayer1GO != nullptr)
			{
				// Player 2 will have the controller 1, so Player 1 will control the other controller 
				// if there is any available
				auto pPlayerInputCP = pPlayer1GO->GetComponent<PlayerInputCP>();
				if (pPlayerInputCP != nullptr)
				{

					pPlayerInputCP->TwoPlayersGameplayInput();
				}
			}
		}
	}
	

	
}

void GameplayState::InitEnemies()
{
	auto& scene = engine::SceneManager::GetInstance().GetActiveScene();

	// FORMATION FOR ALL ENEMIES
	// Formation GO will be the one who creates all enemies with the correct info
	auto go_Formation = std::make_shared<engine::GameObject>(nullptr, LEVEL_TAG, glm::vec3{ 0.f, 0.f, 0.f });
	go_Formation->AddComponent<FormationCP>(go_Formation.get(), "../Data/Formations/Formation1.json");
	go_Formation->AddComponent<AI_FormationCP>(go_Formation.get(), "../Data/Formations/Formation1-Order.json");
	scene.Add(go_Formation);
}

void GameplayState::OnExit()
{

}

GameState* GameplayState::GetChangeState()
{
	return nullptr;
}

void GameplayState::UpdateState(const float)
{

}