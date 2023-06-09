#include "GameOverState.h"
#include "glm/vec3.hpp"
#include "SceneManager.h"
#include "GameObject.h"
#include "ResourceManager.h"
#include "TextComponent.h"
#include "RenderComponent.h"
#include "PlayerScoreCP.h"
#include "Scene.h"
#include "Galaga_Strings.h"
#include <iostream>

GameOverState::~GameOverState()
{

}

void GameOverState::OnEnter()
{
	auto& sceneManager = engine::SceneManager::GetInstance();
	auto& scene = sceneManager.GetActiveScene();
	auto& window = sceneManager.GetSceneWindow();

	auto player = scene.FindGameObjectByTag(STR_PLAYER_TAG, false);
	PlayerScoreCP* pPlayerScore{};
	if (player != nullptr)
	{
		pPlayerScore = player->GetComponent<PlayerScoreCP>();
	}

	glm::vec3 gameOverPos{ (window.width/2.f) - 80.f, (window.height/2.f) - 80.f, 0.f };
	auto galaga_Font = engine::ResourceManager::GetInstance().LoadFont("Fonts/Emulogic-zrEw.ttf", 17);
	SDL_Color blueColor = { 0,255, 222 };

	auto gameOverText{ std::make_shared<engine::GameObject>(nullptr, "UI", gameOverPos) };
	gameOverText->AddComponent<engine::RenderComponent>(gameOverText.get());
	gameOverText->AddComponent<TextComponent>(gameOverText.get(), "GAME OVER", galaga_Font, blueColor);
	gameOverText->SetIsActive(false);

	SDL_Color redColor{ 255, 0, 0 };
	SDL_Color yellowColor{ 255, 255, 0 };
	//glm::vec3 resultsPos{ (window.width / 2.f) - 50.f, (window.height / 2.f) - 50.f, 0.f };
	auto resultsGO{ std::make_shared<engine::GameObject>(nullptr, "UI", gameOverPos)};
	resultsGO->AddComponent<engine::RenderComponent>(resultsGO.get());
	resultsGO->AddComponent<TextComponent>(resultsGO.get(), "-RESULTS-", galaga_Font, redColor);
	resultsGO->SetIsActive(false);


	std::string shotsFired{ "SHOTS FIRED     " };
	if (pPlayerScore != nullptr)
	{
		shotsFired += pPlayerScore->GetShots();
	}
	gameOverPos.x = gameOverPos.x - 100.f;
	gameOverPos.y = gameOverPos.y + 50.f;
	engine::GameObject* pShootsFired{ new engine::GameObject(resultsGO.get(), std::string{"UI"}, gameOverPos) };
	pShootsFired->AddComponent<engine::RenderComponent>(pShootsFired);
	pShootsFired->AddComponent<TextComponent>(pShootsFired, shotsFired, galaga_Font, yellowColor);
	
	std::string numberHits{ "NUMBER OF HITS   " };
	if (pPlayerScore != nullptr)
	{
		numberHits += pPlayerScore->GetHits();
	}
	gameOverPos.y = gameOverPos.y + 50.f;
	engine::GameObject* pNumberHits{ new engine::GameObject(resultsGO.get(), std::string{"UI"}, gameOverPos) };
	pNumberHits->AddComponent<engine::RenderComponent>(pNumberHits);
	pNumberHits->AddComponent<TextComponent>(pNumberHits, numberHits, galaga_Font, yellowColor);

	std::string ratio{ "HIT-MISS RATIO  " };
	if (pPlayerScore != nullptr)
	{
		ratio += pPlayerScore->GetRatio();
		ratio += " %";
	}

	gameOverPos.y = gameOverPos.y + 50.f;
	engine::GameObject* pRatio{ new engine::GameObject(resultsGO.get(), std::string{"UI"}, gameOverPos) };
	pRatio->AddComponent<engine::RenderComponent>(pRatio);
	pRatio->AddComponent<TextComponent>(pRatio, ratio, galaga_Font);

	scene.Add(gameOverText);
	scene.Add(resultsGO);

	m_GameOverObjects.emplace_back(gameOverText.get());
	m_GameOverObjects.emplace_back(resultsGO.get());

}
void GameOverState::OnExit()
{

}

GameState* GameOverState::GetChangeState()
{
	return nullptr;
}

void GameOverState::UpdateState(const float deltaTime)
{
	if (m_CurrentShowing < int(m_GameOverObjects.size()))
	{
		UpdateUIObjects(deltaTime);
	}
}

void GameOverState::UpdateUIObjects(const float deltaTime)
{
	if (m_elapsedSec < m_MaxTime)
	{
		m_elapsedSec += deltaTime;
	}
	else
	{
		if (m_CurrentShowing < int(m_GameOverObjects.size()))
		{
			int previous{ m_CurrentShowing - 1 };
			if (previous >= 0)
			{
				m_GameOverObjects.at(previous)->MarkAsDead();
			}
			m_GameOverObjects.at(m_CurrentShowing)->SetIsActive(true);
			m_CurrentShowing++;
			m_MaxTime = 2.5f;
		}

		m_elapsedSec = 0.f;

	}	
}