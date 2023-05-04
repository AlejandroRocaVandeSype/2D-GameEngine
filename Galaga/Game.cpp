#include "Game.h"
#include "GameIncludes.h"
#include <iostream>

Game::Game(const engine::Window& window)
	: m_Window { window }
{
	Initialize();	
	
}

void Game::Initialize()
{
	auto& scene = engine::SceneManager::GetInstance().CreateScene("FirstStage");	
	SetupBackground();

	// PLAYER 1
	glm::vec3 startPos{ m_Window.width / 2.f, m_Window.height / 1.15f, 0.f };
	auto go_Player = std::make_shared<engine::GameObject>(nullptr, startPos, glm::vec2{ 2.f, 2.f });
	go_Player->AddComponent<engine::RenderComponent>(go_Player.get(), "Player.png");
	go_Player->AddComponent<PlayerInputCP>(go_Player.get());
	go_Player->GetComponent<PlayerInputCP>()->AddControllerMovement(0);
	MoveComponent::Boundaries playerBoundaries{ 0.f, m_Window.width, m_Window.height, 0.f, true };
	go_Player->AddComponent<MoveComponent>(go_Player.get(), 150.f, playerBoundaries);
	

	m_vSceneGObjects.push_back(go_Player);
	MoveComponent::Boundaries missileBoundaries{ 0.f, m_Window.width, m_Window.height, 0.f, false };
	auto go_Missile = std::make_shared<engine::GameObject>(nullptr, startPos, glm::vec2{ 2.f, 2.f });
	go_Missile->AddComponent<engine::RenderComponent>(go_Missile.get(), "PlayerBullet.png");
	go_Missile->AddComponent<MoveComponent>(go_Missile.get(), 300.f, missileBoundaries, glm::vec3{ 0.f, -1.f, 0.f });
	go_Missile->AddComponent<HealthComponent>(go_Missile.get(), 1);
	go_Missile->AddComponent<MissileBehaviourCP>(go_Missile.get());

	m_vSceneGObjects.push_back(go_Missile);

	// Add all the gameObjects to the scene
	for (auto& gameObject : m_vSceneGObjects)
	{
		scene.Add(gameObject);
	}

	// The scene will own the gameObjects
	m_vSceneGObjects.clear();
	
}

// Draw two backgrounds one on top of each other to give a scrolling parallax effect
void Game::SetupBackground()
{
	glm::vec2 backgroundScale{ 0.3f, 0.32f };
	// Background 1
	auto go_Background1 = std::make_shared<engine::GameObject>(nullptr, glm::vec3{ 0.f, 0.f, 0.f },
		backgroundScale);
	go_Background1->AddComponent<engine::RenderComponent>(go_Background1.get(), "background.png");
	go_Background1->AddComponent<ParallaxScrollingCP>(go_Background1.get());
	m_vSceneGObjects.push_back(go_Background1);

	// Background 2 (on top of background1 at start)
	auto backSize = go_Background1->GetComponent<engine::RenderComponent>()->GetTextureSize();
	glm::vec3 background2StartPos{ go_Background1->GetWorldPosition() - glm::vec3{0, backSize.y, 0.f} };

	auto go_Background2 = std::make_shared<engine::GameObject>(nullptr, background2StartPos,
		backgroundScale);
	go_Background2->AddComponent<engine::RenderComponent>(go_Background2.get(), "background.png");
	go_Background2->AddComponent<ParallaxScrollingCP>(go_Background2.get());
	m_vSceneGObjects.push_back(go_Background2);
}

Game::~Game()
{
	std::cout << "Game destructor" << std::endl;
	m_vSceneGObjects.clear();
}

