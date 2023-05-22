#include "Game.h"
#include "GameIncludes.h"
#include <iostream>
#include "EnemyCP.h"

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

	SetupSounds();
	SetupBackground();
	SetupEnemies();


	// PLAYER 1
	glm::vec3 startPos{ m_Window.width / 2.f, m_Window.height / 1.15f, 0.f };
	auto go_Player = std::make_shared<engine::GameObject>(nullptr, PLAYER_TAG, startPos, glm::vec2{2.f, 2.f});
	go_Player->AddComponent<PlayerCP>(go_Player.get(), 3, glm::vec2{m_Window.width, m_Window.height});
	scene.Add(go_Player);


	
}

// Draw two backgrounds one on top of each other to give a scrolling parallax effect
void Game::SetupBackground()
{
	auto& scene = engine::SceneManager::GetInstance().GetActiveScene();

	glm::vec2 backgroundScale{ 0.3f, 0.32f };
	// Background 1
	auto go_Background1 = std::make_shared<engine::GameObject>(nullptr, LEVEL_TAG, glm::vec3{0.f, 0.f, 0.f},
		backgroundScale);
	go_Background1->AddComponent<engine::RenderComponent>(go_Background1.get(), "Sprites/background.png");
	go_Background1->AddComponent<ParallaxScrollingCP>(go_Background1.get());

	scene.Add(go_Background1);

	// Background 2 (on top of background1 at start)
	auto backSize = go_Background1->GetComponent<engine::RenderComponent>()->GetTextureSize();
	glm::vec3 background2StartPos{ go_Background1->GetWorldPosition() - glm::vec3{0, backSize.y, 0.f} };

	auto go_Background2 = std::make_shared<engine::GameObject>(nullptr, LEVEL_TAG, background2StartPos,
		backgroundScale);
	go_Background2->AddComponent<engine::RenderComponent>(go_Background2.get(), "Sprites/background.png");
	go_Background2->AddComponent<ParallaxScrollingCP>(go_Background2.get());

	scene.Add(go_Background2);
}

void Game::SetupSounds()
{
	auto& soundSystem = engine::Servicealocator::Get_Sound_System();

	// Register sounds ID with its path (dont load them yet)
	soundSystem.RegisterSoundID(short(Sounds::startSound), "../Data/Sounds/Start.wav", 50);
	soundSystem.RegisterSoundID(short(Sounds::playerFire), "../Data/Sounds/PlayerShoot.wav", 50);
	soundSystem.RegisterSoundID(short(Sounds::enemyDie), "../Data/Sounds/EnemyDies.wav", 70);

}

void Game::SetupEnemies()
{
	auto& scene = engine::SceneManager::GetInstance().GetActiveScene();

	// FORMATION FOR ALL ENEMIES
	// Formation GO will be the one who creates all enemies with the correct info
	auto go_Formation = std::make_shared<engine::GameObject>(nullptr, LEVEL_TAG, glm::vec3{ 0.f, 0.f, 0.f });
	go_Formation->AddComponent<FormationCP>(go_Formation.get(), "../Data/Formations/Formation1.json");
	go_Formation->AddComponent<AI_FormationCP>(go_Formation.get(), "../Data/Formations/Formation1-Order.json");
	scene.Add(go_Formation);


	auto go_BeeEnemy = std::make_shared<engine::GameObject>(nullptr, "Enemy", glm::vec3{350, 550, 0}, glm::vec2{2.f, 2.f});
	go_BeeEnemy->AddComponent<EnemyCP>(go_BeeEnemy.get(), "Sprites/Bee.png", glm::vec3{0,0,0}, 1);

	scene.Add(go_BeeEnemy);
}

Game::~Game()
{
	std::cout << "Game destructor" << std::endl;
}

