#include "Game.h"
#include "Renderer.h"
#include "ResourceManager.h"
#include "Scene.h"
#include "TextComponent.h"
#include "FPSComponent.h"
#include "RotatorComponent.h"
#include "MoveComponent.h"
#include "HealthComponent.h"
#include "LivesUIComponent.h"
#include "GameObject.h"
#include "InputManager.h"
#include <iostream>

using namespace dae;

Game::Game(const Window& window)
	: m_Window { window }
{

	m_pRenderer = &Renderer::GetInstance();
	m_pSceneManager = &SceneManager::GetInstance();

	if (m_pSceneManager != nullptr)
	{
		m_pScene = &m_pSceneManager->CreateScene("Demo");
	}
	
	// Background 
	auto go_Background = std::make_shared<GameObject>(nullptr, glm::vec3{0.f, 0.f, 0.f});
	go_Background->AddComponent<RenderComponent>(go_Background.get(), "background.tga");
	go_Background->AddComponent<TransformComponent>(go_Background.get());
	m_pScene->Add(go_Background);

	// LOGO
	GameObject* go_Logo = new GameObject(go_Background.get(), glm::vec3{ 216, 180, 0 });
	go_Logo->AddComponent<RenderComponent>(go_Logo, "logo.tga");
	//m_pScene->Add(go_Logo);

	// TEXT TITLE TEXTURE
	GameObject* go_Title = new GameObject(go_Logo, glm::vec3{ 80, 20, 0 });
	//auto go_Title = std::make_shared<GameObject>(glm::vec3{ 80, 20, 0 });
	go_Title->AddComponent<RenderComponent>(go_Title);
	auto font = ResourceManager::GetInstance().LoadFont("Lingua.otf", 30);
	go_Title->AddComponent<TextComponent>(go_Title, "Press 'K' or 'button Y' to kill player ", font);
	//m_pScene->Add(go_Title);
	//go_Title->SetParent(go_Logo);
	

	// FPS TEXT 
	//auto go_FPS = std::make_shared<GameObject>(glm::vec3{ 10, 20, 0 });
	GameObject* go_FPS = new GameObject(go_Logo, glm::vec3{ 10, 20, 0 });
	go_FPS->AddComponent<RenderComponent>(go_FPS);
	font = ResourceManager::GetInstance().LoadFont("Lingua.otf", 15);
	go_FPS->AddComponent<TextComponent>(go_FPS, "Calculating FPS ...", font, SDL_Color{ 0, 255, 0 });
	go_FPS->AddComponent<FPSComponent>(go_FPS);
	//m_pScene->Add(go_FPS);

	//go_FPS->SetParent(go_Logo);
	

	// CONTROLLABLE PLAYER GAMEOBJECT
	glm::vec3 startPos{ window.width / 2.f, window.height / 1.5f, 0.f };
	auto go_Player = std::make_shared<GameObject>(nullptr, startPos, glm::vec2{ 1.5f, 1.5f });
	go_Player->AddComponent<RenderComponent>(go_Player.get(), "Player.png");
	go_Player->AddComponent<HealthComponent>(go_Player.get(), 3);
	go_Player->AddComponent<LivesUIComponent>(go_Player.get(), glm::vec2{10.f, window.height - 50.f});
	go_Player->AddComponent<MoveComponent>(go_Player.get(), 100.f);
	
	m_pScene->Add(go_Player);

	// CONTROLLABLE PLAYER 2 GAMEOBJECT
	glm::vec3 startPos2{ window.width / 2.f, window.height / 2, 0.f };
	auto go_Player2 = std::make_shared<GameObject>(nullptr, startPos2, glm::vec2{ 1.5f, 1.5f });
	go_Player2->AddComponent<RenderComponent>(go_Player2.get(), "Enemy.png");
	go_Player2->AddComponent<HealthComponent>(go_Player2.get(), 3);
	go_Player2->AddComponent<LivesUIComponent>(go_Player2.get(), glm::vec2{ window.width - 90.f, window.height - 50.f });
	go_Player2->AddComponent<MoveComponent>(go_Player2.get(), 200.f, 0);

	m_pScene->Add(go_Player2);












	// UI 
	// - Player Remaining lives
	/*startPos.x = 10.f;
	startPos.y = window.height - 50.f;
	auto go_UILives = std::make_shared<GameObject>(nullptr, startPos, glm::vec2{ 1.5f, 1.5f });
	go_UILives->AddComponent<RenderComponent>(go_UILives.get(), "Player.png");
	go_UILives->AddComponent<HealthComponent>(go_UILives.get(), 3);
	go_UILives->AddComponent<LivesUIComponent>(go_UILives.get());
	//go_UILives->GetComponent<RenderComponent>().
	m_pScene->Add(go_UILives);
	*/

	/*
	// PLayer ship sprite
	//GameObject* go_Player = new GameObject(go_Logo, glm::vec3{ 330, 300, 0 });
	auto go_Player = std::make_shared<GameObject>(nullptr, glm::vec3{ 300, 300, 0 }, glm::vec2{ 1.5f, 1.5f });
	go_Player->AddComponent<RenderComponent>(go_Player.get(), "Player.png");
	go_Player->AddComponent<MoveComponent>(go_Player.get(), 100.f);
	//go_Player->AddComponent<RotatorComponent>(go_Player.get(), 50.f, 3.f);
	//go_Player->AddComponent<TestComponent>(go_Player.get());
	m_pScene->Add(go_Player);

	// Enemy ship sprite
	auto go_Enemy = std::make_shared<GameObject>(nullptr, glm::vec3{ 330, 300, 0 }, glm::vec2{ 1.5f, 1.5f });
	
	//GameObject* go_Enemy = new GameObject(go_Player.get(), glm::vec3{330, 300, 0}, glm::vec2{1.5f, 1.5f});
	go_Enemy->AddComponent<RenderComponent>(go_Enemy.get(), "Enemy.png");
	go_Enemy->AddComponent<MoveComponent>(go_Enemy.get(), 200.f, 0);
	m_pScene->Add(go_Enemy);
	//go_Enemy->AddComponent<RotatorComponent>(go_Enemy, 30.f, 1.f);
	//m_pScene->Add(go_Enemy);

	//go_Enemy->MarkAsDead();
	*/
}

Game::~Game()
{
	std::cout << "Game destructor" << std::endl;
}

void Game::Update(const float deltaTime)
{
	// Update all gameObjects from the scenes
	m_pSceneManager->Update(deltaTime);

}


void Game::Render() const
{
	// Render all the scenes
	m_pRenderer->Render();
}

