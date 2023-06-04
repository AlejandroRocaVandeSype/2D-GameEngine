#include "GameplayState.h"
#include "GameObject.h"
#include "PlayerCP.h"
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

		// PLAYER 1
		glm::vec3 startPos{ window.width / 2.f, window.height / 1.15f, 0.f };
		auto go_Player = std::make_shared<engine::GameObject>(nullptr, PLAYER_TAG, startPos, glm::vec2{ 2.f, 2.f });
		go_Player->AddComponent<PlayerCP>(go_Player.get(), 3, glm::vec2{ window.width, window.height });
		scene.Add(go_Player);
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