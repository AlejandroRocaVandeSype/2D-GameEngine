#include "MenuState.h"
#include "GameplayState.h"
#include "GameObject.h"
#include "SceneManager.h"
#include "Scene.h"
#include "ParallaxScrollingCP.h"
#include "InputManager.h"
#include "StartCommand.h"


MenuState::~MenuState() 
{
	std::cout << "Menu State destructor" << std::endl;
}

void MenuState::OnEnter()
{
	InitBackground();
	InitMenuUI();
	InitMenuInput();
}

// Draw two backgrounds one on top of each other to give a scrolling parallax effect
// They wont move until Game starts
void MenuState::InitBackground()
{
	auto& scene = engine::SceneManager::GetInstance().GetActiveScene();

	glm::vec2 backgroundScale{ 0.3f, 0.32f };
	// Background 1
	auto go_Background1 = std::make_shared<engine::GameObject>(nullptr, "Level", glm::vec3{ 0.f, 0.f, 0.f },
		backgroundScale);
	go_Background1->AddComponent<engine::RenderComponent>(go_Background1.get(), "Sprites/background.png");
	go_Background1->AddComponent<ParallaxScrollingCP>(go_Background1.get());

	scene.Add(go_Background1);

	// Background 2 (on top of background1 at start)
	auto backSize = go_Background1->GetComponent<engine::RenderComponent>()->GetTextureSize();
	glm::vec3 background2StartPos{ go_Background1->GetWorldPosition() - glm::vec3{0, backSize.y, 0.f} };

	auto go_Background2 = std::make_shared<engine::GameObject>(nullptr, "Level", background2StartPos,
		backgroundScale);
	go_Background2->AddComponent<engine::RenderComponent>(go_Background2.get(), "Sprites/background.png");
	go_Background2->AddComponent<ParallaxScrollingCP>(go_Background2.get());

	scene.Add(go_Background2);
	
	m_vBackgrounds.push_back(go_Background1);
	m_vBackgrounds.push_back(go_Background2);

}

// Set all the Input in order to be able to navigate through the menu
void MenuState::InitMenuInput()
{
	auto& input = engine::InputManager::GetInstance();

	SDL_KeyCode keyEnter{ SDLK_RETURN };
	std::unique_ptr<Command> startCommand = std::make_unique<StartCommand>(this);

	input.BindCommand(std::move(startCommand), keyEnter, engine::InputType::Pressed);
}

void MenuState::InitMenuUI()
{
	auto& sceneManager = engine::SceneManager::GetInstance();
	auto& scene = sceneManager.GetActiveScene();
	auto& window = sceneManager.GetSceneWindow();

	glm::vec3 titlePos{ (window.width / 2.f) - 121.f, window.height / 4.f, 0.f };

	m_Title = std::make_shared<engine::GameObject>(nullptr, "UI", titlePos, glm::vec2{ 2.f, 2.f });
	m_Title->AddComponent<engine::RenderComponent>(m_Title.get(), "Sprites/Title.png");

	scene.Add(m_Title);
}

void MenuState::OnExit()
{
	auto& input = engine::InputManager::GetInstance();
	input.UnbindAllCommands();

	m_Title->MarkAsDead();

	// Activate the parallax scrolling for the background
	for (auto& gameObject : m_vBackgrounds)
	{
		auto scrollingCP = gameObject->GetComponent<ParallaxScrollingCP>();

		if (scrollingCP != nullptr)
		{
			scrollingCP->ActivateScrolling();
		}
	}

}

GameState* MenuState::GetChangeState()
{
	if (m_ChangeState)
	{
		OnExit();
		return new GameplayState();
	}
	return nullptr;
}

void MenuState::UpdateState(const float )
{
	
}

void MenuState::StartGame()
{
	// New State
	m_ChangeState = true;
}