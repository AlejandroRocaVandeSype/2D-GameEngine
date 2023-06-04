#include "MenuState.h"
#include "GameplayState.h"
#include "GameObject.h"
#include "SceneManager.h"
#include "Scene.h"
#include "ParallaxScrollingCP.h"
#include "InputManager.h"
#include "MenuSelectionCommand.h"
#include "TextComponent.h"
#include "ResourceManager.h"
#include "MenuSelectionCP.h"


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
	
	m_vMenuGO.push_back(go_Background1);
	m_vMenuGO.push_back(go_Background2);

}

// Set all the Input in order to be able to navigate through the menu
void MenuState::InitMenuInput()
{
	auto& input = engine::InputManager::GetInstance();

	SDL_KeyCode keyEnter{ SDLK_RETURN };
	SDL_KeyCode keyArrowDown{ SDLK_DOWN };
	SDL_KeyCode keyArrowUP{ SDLK_UP };

	std::unique_ptr<Command> upSelectionCommand = std::make_unique<MenuSelectionCommand>(m_pMenuSelectionCP ,-1);
	std::unique_ptr<Command> downSelectionCommand = std::make_unique<MenuSelectionCommand>(m_pMenuSelectionCP, 1);
	std::unique_ptr<Command> selectOption = std::make_unique<MenuSelectionCommand>(m_pMenuSelectionCP, 0);

	input.BindCommand(std::move(upSelectionCommand), keyArrowUP, engine::InputType::Down);
	input.BindCommand(std::move(downSelectionCommand), keyArrowDown, engine::InputType::Down);
	input.BindCommand(std::move(selectOption), keyEnter, engine::InputType::Down);

}

void MenuState::InitMenuUI()
{
	auto& sceneManager = engine::SceneManager::GetInstance();
	auto& scene = sceneManager.GetActiveScene();
	auto& window = sceneManager.GetSceneWindow();

	// TITTLE
	glm::vec3 titlePos{ (window.width / 2.f) - 121.f, window.height / 4.f, 0.f };
	auto title = std::make_shared<engine::GameObject>(nullptr, "UI", titlePos, glm::vec2{ 2.f, 2.f });
	title->AddComponent<engine::RenderComponent>(title.get(), "Sprites/Title.png");
	scene.Add(title);
	m_vMenuGO.push_back(title);

	// OPTIONS
	auto galaga_Font = engine::ResourceManager::GetInstance().LoadFont("Fonts/Emulogic-zrEw.ttf", 18);
	std::vector<std::pair<std::string, glm::vec3>> menuOptions;		// Keep track of the option positions for the selection

		// 1 PLAYER
	glm::vec3 optionPos{ titlePos.x + 40, titlePos.y + 150.f, 0.f };
	auto one_player_opt{ std::make_shared<engine::GameObject>(nullptr, "UI", optionPos) };
	one_player_opt->AddComponent<engine::RenderComponent>(one_player_opt.get());
	one_player_opt->AddComponent<TextComponent>(one_player_opt.get(), ONE_PLAYER_OPT, galaga_Font);
	menuOptions.push_back(std::make_pair(ONE_PLAYER_OPT, optionPos));

		// 2 PLAYERS
	optionPos.x = optionPos.x;
	optionPos.y = optionPos.y + 40.f;
	auto two_players_opt{ std::make_shared<engine::GameObject>(nullptr, "UI", optionPos)};
	two_players_opt->AddComponent<engine::RenderComponent>(two_players_opt.get());
	two_players_opt->AddComponent<TextComponent>(two_players_opt.get(), TWO_PLAYERS_OPT, galaga_Font);
	menuOptions.push_back(std::make_pair(TWO_PLAYERS_OPT, optionPos));

		// VERSUS
	optionPos.x = optionPos.x + 30.f;
	optionPos.y = optionPos.y + 40.f;
	auto versus_opt{ std::make_shared<engine::GameObject>(nullptr, "UI", optionPos) };
	versus_opt->AddComponent<engine::RenderComponent>(versus_opt.get());
	versus_opt->AddComponent<TextComponent>(versus_opt.get(), VERSUS_OPT, galaga_Font);
	menuOptions.push_back(std::make_pair(VERSUS_OPT, optionPos));

		// CONTROLS
	optionPos.x = optionPos.x - 20.f;
	optionPos.y = optionPos.y + 40.f;
	auto controls_opt{ std::make_shared<engine::GameObject>(nullptr, "UI", optionPos) };
	controls_opt->AddComponent<engine::RenderComponent>(controls_opt.get());
	controls_opt->AddComponent<TextComponent>(controls_opt.get(), CONTROLS_OPT, galaga_Font);
	menuOptions.push_back(std::make_pair(CONTROLS_OPT, optionPos));

	// OPTION SELECTION ARROW
	optionPos.x = optionPos.x - 10.f;
	optionPos.y = optionPos.y - 120.f;
	auto arrow_opt { std::make_shared<engine::GameObject>(nullptr, "UI", glm::vec3{optionPos.x - 30.f, optionPos.y, 0.f}) };
	arrow_opt->AddComponent<engine::RenderComponent>(arrow_opt.get());
	arrow_opt->AddComponent<TextComponent>(arrow_opt.get(), ">", galaga_Font);
	m_pMenuSelectionCP = arrow_opt->AddComponent<MenuSelectionCP>(arrow_opt.get(), menuOptions);


	scene.Add(one_player_opt);
	scene.Add(two_players_opt);
	scene.Add(versus_opt);
	scene.Add(controls_opt);
	scene.Add(arrow_opt);
	m_vMenuGO.push_back(two_players_opt);
	m_vMenuGO.push_back(one_player_opt);
	m_vMenuGO.push_back(versus_opt);
	m_vMenuGO.push_back(controls_opt);
	m_vMenuGO.push_back(arrow_opt);
}

void MenuState::OnExit()
{
	auto& input = engine::InputManager::GetInstance();
	input.UnbindAllCommands();

	for (auto& gameObject : m_vMenuGO)
	{
		auto scrollingCP = gameObject->GetComponent<ParallaxScrollingCP>();
		if (scrollingCP != nullptr)
		{
			// Activate the parallax scrolling for the background
			scrollingCP->ActivateScrolling();
		}
		else
		{
			// If not background is a gameObject that needs to be destroyed
			gameObject->MarkAsDead();
		}	
	}

}

GameState* MenuState::GetChangeState()
{
	if (m_pMenuSelectionCP != nullptr && m_pMenuSelectionCP->IsOptionSelected())
	{
		std::string selectedOption{ m_pMenuSelectionCP->GetSelection() };

		if (selectedOption == ONE_PLAYER_OPT)
		{
			OnExit();
			return new GameplayState(selectedOption);
		}	
		
		m_pMenuSelectionCP->SetOptionSelected(false);
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