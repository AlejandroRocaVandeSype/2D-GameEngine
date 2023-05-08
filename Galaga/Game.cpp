#include "Game.h"
#include "GameIncludes.h"
#include <iostream>

Game::Game(const engine::Window& window)
	: FIRST_STAGE { "FirstStage "}
	, SECOND_STAGE{ "SecondStage " }
	, THIRD_STAGE{ "ThirdStage " }
	, PLAYER_TAG { "Player" }
	, ENEMY_TAG { "Enemy" }
	, LEVEL_TAG { "Level" }
	, UI_TAG { "UI" }
	, m_Window { window }
{
	Initialize();	
	
}

void Game::Initialize()
{
	// Setup all available scenes and the active scene
	auto& sceneManager = engine::SceneManager::GetInstance();
	sceneManager.CreateScene(FIRST_STAGE);
	sceneManager.CreateScene(SECOND_STAGE);
	sceneManager.CreateScene(THIRD_STAGE);
	sceneManager.SetActiveScene(FIRST_STAGE);

	auto& scene = sceneManager.GetActiveScene();

	SetupBackground();

	// PLAYER 1
	glm::vec3 startPos{ m_Window.width / 2.f, m_Window.height / 1.15f, 0.f };
	auto go_Player = std::make_shared<engine::GameObject>(nullptr, PLAYER_TAG, startPos, glm::vec2{2.f, 2.f});
	go_Player->AddComponent<PlayerCP>(go_Player.get(), 1, glm::vec2{m_Window.width, m_Window.height});
	scene.Add(go_Player);

	// ENEMY
	glm::vec3 startPos2{ m_Window.width / 3.f, 200.f, 0.f };
	auto go_Enemy = std::make_shared<engine::GameObject>(nullptr, ENEMY_TAG, startPos2, glm::vec2{ 2.f, 2.f });
	go_Enemy->AddComponent<EnemyCP>(go_Enemy.get(), 1);
	scene.Add(go_Enemy);

	glm::vec3 startPos3{ m_Window.width / 1.5f, 550.f, 0.f };
	auto go_Enemy2 = std::make_shared<engine::GameObject>(nullptr, ENEMY_TAG, startPos3, glm::vec2{ 2.f, 2.f });
	go_Enemy2->AddComponent<EnemyCP>(go_Enemy2.get(), 1);
	scene.Add(go_Enemy2);

	glm::vec3 startPos4{ m_Window.width / 1.1f, 550.f, 0.f };
	auto go_Enemy3 = std::make_shared<engine::GameObject>(nullptr, ENEMY_TAG, startPos4, glm::vec2{ 2.f, 2.f });
	go_Enemy3->AddComponent<EnemyCP>(go_Enemy3.get(), 1);
	scene.Add(go_Enemy3);

	//go_Player->SetIsActive(false);
	
	
}

// Draw two backgrounds one on top of each other to give a scrolling parallax effect
void Game::SetupBackground()
{
	auto& scene = engine::SceneManager::GetInstance().GetActiveScene();

	glm::vec2 backgroundScale{ 0.3f, 0.32f };
	// Background 1
	auto go_Background1 = std::make_shared<engine::GameObject>(nullptr, LEVEL_TAG, glm::vec3{0.f, 0.f, 0.f},
		backgroundScale);
	go_Background1->AddComponent<engine::RenderComponent>(go_Background1.get(), "background.png");
	go_Background1->AddComponent<ParallaxScrollingCP>(go_Background1.get());

	scene.Add(go_Background1);

	// Background 2 (on top of background1 at start)
	auto backSize = go_Background1->GetComponent<engine::RenderComponent>()->GetTextureSize();
	glm::vec3 background2StartPos{ go_Background1->GetWorldPosition() - glm::vec3{0, backSize.y, 0.f} };

	auto go_Background2 = std::make_shared<engine::GameObject>(nullptr, LEVEL_TAG, background2StartPos,
		backgroundScale);
	go_Background2->AddComponent<engine::RenderComponent>(go_Background2.get(), "background.png");
	go_Background2->AddComponent<ParallaxScrollingCP>(go_Background2.get());

	scene.Add(go_Background2);
}

Game::~Game()
{
	std::cout << "Game destructor" << std::endl;
}

