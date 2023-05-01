#include "Game.h"
#include "Renderer.h"
#include "ResourceManager.h"
#include "Scene.h"
#include "GameObject.h"
#include "ParallaxScrollingCP.h"
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

