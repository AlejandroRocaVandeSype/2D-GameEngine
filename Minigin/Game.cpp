#include "Game.h"
#include "SceneManager.h"
#include "Renderer.h"
#include "ResourceManager.h"
#include "Scene.h"
#include "TextComponent.h"
#include "FPSComponent.h"
#include "TextObject.h"

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
	go = std::make_shared<GameObject>();
	go->AddComponent<RenderComponent>(go.get(), "background.tga");
	m_pScene->Add(go);

	// Previous gameObject is not deleted since inside the scene there is still a shared_ptr owning the original one
	// LOGO
	go = std::make_shared<GameObject>(glm::vec3{ 216, 180, 0 });
	go->AddComponent<RenderComponent>(go.get(), "logo.tga");
	m_pScene->Add(go);
	
	// TEXT TITLE TEXTURE
	go = std::make_shared<GameObject>(glm::vec3{ 80, 20, 0 });
	go->AddComponent<RenderComponent>(go.get());
	auto font = ResourceManager::GetInstance().LoadFont("Lingua.otf", 36);
	go->AddComponent<TextComponent>(go.get(), "Programming 4 Assignment", font);
	m_pScene->Add(go);

	// FPS TEXT 
	go = std::make_shared<GameObject>(glm::vec3{ 10, 20, 0 });
	go->AddComponent<RenderComponent>(go.get());
	font = ResourceManager::GetInstance().LoadFont("Lingua.otf", 15);
	go->AddComponent<TextComponent>(go.get(), "Calculating FPS ...", font, SDL_Color{ 0, 255, 0 });
	go->AddComponent<FPSComponent>(go.get());
	m_pScene->Add(go);

	//go->GameObject()->SetIsDead(true);
	//go->GameObject()->RemoveComponent<RenderComponent>();
	

	/*
	auto font = ResourceManager::GetInstance().LoadFont("Lingua.otf", 15);
	auto fpsObject = std::make_shared<TextObject>("0 FPS", font, glm::vec3{ 10, 20, 0 }, SDL_Color{ 0, 255, 0 });
	fpsObject->AddFPSComponent();
	m_pScene->Add(fpsObject->GameObject());

	//fpsObject->GameObject()->RemoveComponent<TextComponent>();
	
	*/

		
	
	/*
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
	
	*/
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

