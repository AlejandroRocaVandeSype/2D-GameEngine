#include "Game.h"
#include "SceneManager.h"
#include "Renderer.h"
#include "ResourceManager.h"
#include "Scene.h"


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
	go = std::make_shared<TextureObject>("background.tga");
	m_pScene->Add(go->GameObject());

	// Logo
	go = std::make_shared<TextureObject>("logo.tga", glm::vec3{ 216, 180, 0 });
	m_pScene->Add(go->GameObject());

	// Title text
	auto font = ResourceManager::GetInstance().LoadFont("Lingua.otf", 36);
	to = std::make_shared<TextObject>("Programming 4 Assignment", font, glm::vec3{ 80, 20, 0 });
	m_pScene->Add(to->GameObject());

	// FPS text counter
	font = ResourceManager::GetInstance().LoadFont("Lingua.otf", 15);
	fpsObject = std::make_shared<TextObject>("0 FPS", font, glm::vec3{ 10, 20, 0 }, SDL_Color{0, 255, 0});
	fpsObject->AddFPSComponent();
	m_pScene->Add(fpsObject->GameObject());
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

