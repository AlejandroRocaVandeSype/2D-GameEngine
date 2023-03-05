#include "Game.h"
#include "SceneManager.h"
#include "Renderer.h"
#include "ResourceManager.h"
#include "Scene.h"
#include "TextComponent.h"
#include "FPSComponent.h"
#include "RotatorComponent.h"
#include "GameObject.h"

using namespace dae;

Game::Game()
{
	m_pRenderer = &Renderer::GetInstance();
	m_pSceneManager = &SceneManager::GetInstance();

	if (m_pSceneManager != nullptr)
	{
		m_pScene = &m_pSceneManager->CreateScene("Demo");
	}
	
	// Background 
	
	auto go_Background = std::make_shared<GameObject>();
	go_Background->AddComponent<RenderComponent>(go_Background.get(), "background.tga");
	go_Background->AddComponent<TransformComponent>(go_Background.get());
	m_pScene->Add(go_Background);
	
	// Previous gameObject is not deleted since inside the scene there is still a shared_ptr owning the original one
	// LOGO
	
	auto go_Logo = std::make_shared<GameObject>(glm::vec3{ 216, 180, 0 });
	go_Logo->AddComponent<RenderComponent>(go_Logo.get(), "logo.tga");
	m_pScene->Add(go_Logo);
	
	// TEXT TITLE TEXTURE
	auto go_Title = std::make_shared<GameObject>(glm::vec3{ 80, 20, 0 });
	go_Title->AddComponent<RenderComponent>(go_Title.get());
	auto font = ResourceManager::GetInstance().LoadFont("Lingua.otf", 36);
	go_Title->AddComponent<TextComponent>(go_Title.get(), "Programming 4 Assignment", font);
	m_pScene->Add(go_Title);

	// FPS TEXT 
	auto go_FPS = std::make_shared<GameObject>(glm::vec3{ 10, 20, 0 });
	go_FPS->AddComponent<RenderComponent>(go_FPS.get());
	font = ResourceManager::GetInstance().LoadFont("Lingua.otf", 15);
	go_FPS->AddComponent<TextComponent>(go_FPS.get(), "Calculating FPS ...", font, SDL_Color{ 0, 255, 0 });
	go_FPS->AddComponent<FPSComponent>(go_FPS.get());
	m_pScene->Add(go_FPS);
	
	auto go_Player = std::make_shared<GameObject>(glm::vec3{ 200, 300, 0 });
	go_Player->AddComponent<RenderComponent>(go_Player.get(), "Player.png");
	go_Player->AddComponent<RotatorComponent>(go_Player.get());
	m_pScene->Add(go_Player);

	//go_Player->SetParent(go_Title.get());
	//go_FPS->SetParent(go_Player.get(), true);

	auto go_Enemy = std::make_shared<GameObject>(glm::vec3{ 330, 300, 0 });
	go_Enemy->SetParent(go_Player.get(), true);
	go_Enemy->AddComponent<RenderComponent>(go_Enemy.get(), "Enemy.png");
	go_Enemy->AddComponent<RotatorComponent>(go_Enemy.get());
	m_pScene->Add(go_Enemy);
	
	
	//go_Player->SetIsDead(true);
	
}

Game::~Game()
{
	
}

void Game::Update([[maybe_unused]] const float deltaTime)
{
	// Update all gameObjects from the scenes
	m_pSceneManager->Update(deltaTime);

}


void Game::Render() const
{
	// Render all the scenes
	m_pRenderer->Render();
}

