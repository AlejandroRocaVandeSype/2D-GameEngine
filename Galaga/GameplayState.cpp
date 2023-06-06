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
	if (m_GameMode == "1 PLAYER")
	{
		std::cout << "1 PLAYER MODE" << std::endl;

		auto& sceneManager = engine::SceneManager::GetInstance();
		auto& scene = sceneManager.GetActiveScene();

		auto& window = sceneManager.GetSceneWindow();

		InitEnemies();

		// Add the PlayerCP to Player gameObject and the input for the Gameplay
		auto pPlayerGO = scene.FindGameObjectByTag("Player");
		if (pPlayerGO != nullptr)
		{
			pPlayerGO->AddComponent<PlayerCP>(pPlayerGO, 3, glm::vec2{ window.width, window.height });
			auto pPlayerInputCP = pPlayerGO->GetComponent<PlayerInputCP>();
			if (pPlayerInputCP != nullptr)
			{
				pPlayerInputCP->AddGameplayInput();
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