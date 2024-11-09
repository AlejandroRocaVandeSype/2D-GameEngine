#include "GameplayState.h"
#include "GameObject.h"
#include "../Components/Player/PlayerCP.h"
#include "../Components/Player/PlayerInputCP.h"
#include "Scene.h"
#include "../Components/Enemies/FormationCP.h"
#include "../Components/Enemies/AI_FormationCP.h"
#include "../Galaga_Strings.h"
#include "GameOverState.h"
#include "../Components/Enemies/EnemyCP.h"
#include "Servicealocator.h"
#include "../Components/Sound/SoundIDs.h"
#include "../Components/UI/TextComponent.h"
#include "ResourceManager.h"
#include "InputManager.h"
#include "../Commands/SkipLevelCommand.h"


GameplayState::GameplayState(const std::string& gameMode)
	:m_GameMode { gameMode }, m_pFormationCP { nullptr }, m_vEnemiesData{ }
	, m_IsGameOver{ false }, m_IsChangingState{ true }, m_ElapsedTime { 0.f }
	, MAX_TIME_CHANGE { 3.f }, m_CurrentStage{FIRST_STAGE}
{
	// Load all data paths 
	LoadDataPaths();
}

void GameplayState::LoadDataPaths()
{
	FormationJsonData formationData("Data/Formations/Formation1.json", "Data/Formations/Formation1-Order.json");
	std::pair stageData{ std::make_pair(FIRST_STAGE, formationData) };
	m_vEnemiesData.emplace_back(stageData);

	formationData.changeData("Data/Formations/Formation3.json", "Data/Formations/Formation3-Order.json");
	stageData.first = SECOND_STAGE;
	stageData.second = formationData;
	m_vEnemiesData.emplace_back(stageData);

	formationData.changeData("Data/Formations/Formation2.json", "Data/Formations/Formation2-Order.json");
	stageData.first = THIRD_STAGE;
	stageData.second = formationData;
	m_vEnemiesData.emplace_back(stageData);
}

GameplayState::~GameplayState()
{
}

void GameplayState::OnEnter()
{

	auto& sceneManager = engine::SceneManager::GetInstance();
	auto& scene = sceneManager.GetActiveScene();
	auto& window = sceneManager.GetSceneWindow();

	// STAGE INFO TEXT
	auto galaga_Font = engine::ResourceManager::GetInstance().LoadFont("Fonts/Emulogic-zrEw.ttf", 18);
	SDL_Color blueColor = { 0,255, 222 };
	glm::vec3 stageInfoPos{ (window.width / 2.f) - 60.f, (window.height / 2.f) - 80.f, 0.f };
	m_StageInfoText = std::make_shared<engine::GameObject>(nullptr, "UI", stageInfoPos);
	m_StageInfoText->AddComponent<engine::RenderComponent>(m_StageInfoText.get());
	m_StageInfoText->AddComponent<TextComponent>(m_StageInfoText.get(), FIRST_STAGE, galaga_Font, blueColor);
	m_StageInfoText->SetIsActive(false);
	scene.Add(m_StageInfoText);

	auto& soundSystem = engine::Servicealocator::Get_Sound_System();
	soundSystem.PlaySound(short(Sounds::startSound));

	if (m_GameMode == "1 PLAYER" || m_GameMode == "2 PLAYERS")
	{
		InitPlayer1();
		if (m_GameMode == "2 PLAYERS")
		{
			// Add Player 2 too ( Player 2 will use the available controller)
			InitPlayer2();
		}

		InitEnemies();
	}
		
}

void GameplayState::InitEnemies()
{
	auto& scene = engine::SceneManager::GetInstance().GetActiveScene();

	// FORMATION FOR ALL ENEMIES
	// Formation GO will be the one who creates all enemies with the correct info
	auto go_Formation = std::make_shared<engine::GameObject>(nullptr, STR_LEVEL_TAG, glm::vec3{ 0.f, 0.f, 0.f });
	m_pFormationCP = go_Formation->AddComponent<FormationCP>(go_Formation.get(), m_vEnemiesData.at(0).second.GetEnemiesData());
	go_Formation->AddComponent<AI_FormationCP>(go_Formation.get(), m_vEnemiesData.at(0).second.GetSpawnOrderData());
	scene.Add(go_Formation);
	
}

void GameplayState::InitPlayer1()
{
	auto& sceneManager = engine::SceneManager::GetInstance();
	auto& scene = sceneManager.GetActiveScene();
	auto& window = sceneManager.GetSceneWindow();
	auto& input = engine::InputManager::GetInstance();

	// Add the PlayerCP to Player gameObject and the input for the Gameplay
	auto pPlayer1GO = scene.FindGameObjectByTag(STR_PLAYER_TAG);
	if (pPlayer1GO != nullptr)
	{

		// Add also a command to be able to Skip levels with the keyboard
		SDL_KeyCode key_F1{ SDLK_F1 };
		std::unique_ptr<Command> skipLevelCommand = std::make_unique<SkipLevelCommand>(this);
		input.BindCommand(std::move(skipLevelCommand), key_F1, engine::InputType::Up);

		m_vPlayers.emplace_back(pPlayer1GO);
		pPlayer1GO->AddComponent<PlayerCP>(pPlayer1GO, 4, 1, glm::vec2{ window.width, window.height });

		auto pPlayerInputCP = pPlayer1GO->GetComponent<PlayerInputCP>();
		if (pPlayerInputCP != nullptr && m_GameMode == "1 PLAYER")
		{
			// Player 1 can play with Keyboard and Controller
			pPlayerInputCP->GameplayControllerInput(unsigned int(0));
			pPlayerInputCP->GameplayKeyboardInput();
			return;
		}
	}
}
void GameplayState::InitPlayer2()
{

	auto& sceneManager = engine::SceneManager::GetInstance();
	auto& scene = sceneManager.GetActiveScene();
	auto& window = sceneManager.GetSceneWindow();

	glm::vec3 startPos{ (window.width / 2.f) + 50.f, window.height / 1.15f, 0.f };
	auto go_Player2 = std::make_shared<engine::GameObject>(nullptr, STR_PLAYER_TAG, startPos, glm::vec2{ 2.f, 2.f });
	go_Player2->AddComponent<PlayerCP>(go_Player2.get(), 4, 2, glm::vec2{ window.width, window.height });
	auto pPlayer2InputCP = go_Player2->AddComponent<PlayerInputCP>(go_Player2.get());
	pPlayer2InputCP->TwoPlayersGameplayInput();

	auto pPlayer1 = scene.FindGameObjectByTag(STR_PLAYER_TAG);
	if (pPlayer1 != nullptr)
	{
		// Player 2 will have the controller 1, so Player 1 will control the other controller 
		// if there is any available
		auto pPlayerInputCP = pPlayer1->GetComponent<PlayerInputCP>();
		if (pPlayerInputCP != nullptr)
		{
			pPlayerInputCP->TwoPlayersGameplayInput();
		}
	}

	scene.Add(go_Player2);
	m_vPlayers.emplace_back(go_Player2.get());
}

bool GameplayState::NextStage()
{
	if (m_pFormationCP != nullptr)
	{
		auto& sceneManager = engine::SceneManager::GetInstance();
		if (sceneManager.LoadNextScene())
		{
			// New scene loaded succesfully -> Reset enemies data
			for (size_t enemiesIdx{ 0 }; enemiesIdx < m_vEnemiesData.size(); ++enemiesIdx)
			{
				auto enemiesData = m_vEnemiesData.at(enemiesIdx);
				if (enemiesData.first == sceneManager.GetActiveSceneName())
				{
					// Get the corresponding data from this new stage
					m_pFormationCP->Reset(enemiesData.second.GetEnemiesData(), enemiesData.second.GetSpawnOrderData());
					return true;
				}
			}

		}
	}
		
	// No more scenes or no data found 
	return false;

}

void GameplayState::SkipStage()
{
	m_pFormationCP->DeactivateAllEnemies();
	
	if (!m_IsChangingState)
	{
		// Avoid keep reset the time everytime the player skip stage
		m_ElapsedTime = 0;
	}
	auto& sceneManager = engine::SceneManager::GetInstance();
	auto nextSceneName = sceneManager.GetNextSceneName();
	if (nextSceneName != "No more scenes")
	{
		m_CurrentStage = nextSceneName;
		m_StageInfoText->GetComponent<TextComponent>()->SetText(nextSceneName);
		m_IsChangingState = true;
	}
}

void GameplayState::OnExit()
{
	m_StageInfoText->MarkAsDead();
	auto& input = engine::InputManager::GetInstance();
	input.UnbindAllCommands();
}

GameState* GameplayState::GetChangeState()
{
	if (m_IsGameOver)
	{
		//OnExit();
		return new GameOverState();
	}

	return nullptr;
}

void GameplayState::UpdateState(const float deltaTime)
{
	if (m_IsChangingState)
	{
		UpdateChangingStage(deltaTime);
	}
	else if(m_pFormationCP!=nullptr)
	{
		if (ArePlayersAlive())
		{
			// Player(s) still alive -> Check if there are enemies left
			if (!m_pFormationCP->AreEnemiesLeft())
			{
				if (m_IsChangingState == false)
				{
					// wait a few seconds before loading enemies from next stage
					auto& sceneManager = engine::SceneManager::GetInstance();
					if (sceneManager.GetNextSceneName() != "No more scenes")
					{
						m_StageInfoText->GetComponent<TextComponent>()->SetText(sceneManager.GetNextSceneName());
						m_IsChangingState = true;
					}
					else
					{
						// No more scenes
						m_IsGameOver = true;
					}					
					return;
				}
			}
		}
		else
		{
			m_IsGameOver = true;
		}
	}
}

void GameplayState::UpdateChangingStage(const float deltaTime)
{
	// Show stage number info
	m_StageInfoText->SetIsActive(true);
	m_ElapsedTime += deltaTime;

	if (m_ElapsedTime > MAX_TIME_CHANGE)
	{
		// When time over load enemies from the next stage
		m_StageInfoText->SetIsActive(false);
		m_IsChangingState = false;
		m_ElapsedTime = 0.f;
		if (m_CurrentStage == FIRST_STAGE)
		{
			// If first stage -> Enemies already loaded
			m_pFormationCP->SpawnEnemies();
			m_CurrentStage = SECOND_STAGE;
			return;
		}

		if (m_pFormationCP != nullptr)
		{
			// If nextStage returns false then we go the GameOver state
			if (!NextStage())
			{
				m_IsGameOver = true;
			}
			m_pFormationCP->SpawnEnemies();
		}

	}
}

bool GameplayState::ArePlayersAlive()
{
	for (const auto& player : m_vPlayers)
	{
		if (player != nullptr && player->IsActive())
		{
			// At least one player is still alive
			return true;
		}
	}

	return false;
}