#include "PlayerScoreCP.h"
#include "GameObject.h"
#include "Component.h"
#include "TextComponent.h"
#include "SceneManager.h"
#include "ResourceManager.h"
#include "MissileCP.h"
#include <iostream>
#include <iomanip> // for std::setprecision
#include <sstream>

PlayerScoreCP::PlayerScoreCP(engine::GameObject* pOwner, unsigned int playerIdx)
	: Component("PlayerScoreCP", pOwner)
	, m_pTextComponent { nullptr }, m_PlayerHits { 0 }, m_PlayerShots { 0 }
{
	auto& sceneManager = engine::SceneManager::GetInstance();
	auto& window = sceneManager.GetSceneWindow();

	glm::vec3 scorePos{ 0.f, 20.f, 0.f };
	glm::vec3 playerSymbolPos{ 0.f, 0.f, 0.f };
	std::string playerSymbol{ };

	if (playerIdx == 1)
	{
		// Left side of the screen
		scorePos.x = 40.f;
		playerSymbolPos.x = 50.f;
		playerSymbol = "1UP";
	}
	else
	{
		// Right side of the screen
		scorePos.x = window.width - 90.f;
		playerSymbolPos.x = window.width - 100.f;
		playerSymbol = "2UP";
	}

	auto galaga_Font = engine::ResourceManager::GetInstance().LoadFont("Fonts/Emulogic-zrEw.ttf", 17);
	engine::GameObject* pScore = new engine::GameObject(pOwner, std::string{ "UI" }, scorePos);
	pScore->AddComponent<engine::RenderComponent>(pScore);
	m_pTextComponent = pScore->AddComponent<TextComponent>(pScore, "00", galaga_Font);


	engine::GameObject* pPlayerSymbol = new engine::GameObject(pOwner, std::string{ "UI" }, playerSymbolPos);
	pPlayerSymbol->AddComponent<engine::RenderComponent>(pPlayerSymbol);
	SDL_Color redColor{ 255, 0, 0 };
	pPlayerSymbol->AddComponent<TextComponent>(pPlayerSymbol, playerSymbol, galaga_Font, redColor);

	
}

PlayerScoreCP::~PlayerScoreCP()
{
	std::cout << "PlayerScore destructor" << std::endl;
}

void PlayerScoreCP::Update(const float)
{

}

void PlayerScoreCP::UpdateScore(int points)
{

	if (m_pTextComponent != nullptr)
	{
		int currentScore{ std::stoi(m_pTextComponent->GetText()) };

		currentScore += points;

		std::string newScore{ std::to_string(currentScore) };

		m_pTextComponent->SetText(newScore);
	}

	if (points > 0)
	{
		m_PlayerHits++;
	}

}

void PlayerScoreCP::ReceiveMessage(const std::string& message, const std::string& value)
{
	if (message == "RemoveCP")
	{
		if (value == "TextCP")
		{
			m_pTextComponent = nullptr;
		}
	}
}

void PlayerScoreCP::OnNotify(engine::GameObject* gameObject, const engine::Event& event)
{
	if (event.IsSameEvent("UpdatePoints") )
	{
		auto missileCP = gameObject->GetComponent<MissileCP>();
		if (missileCP != nullptr)
		{
			UpdateScore(missileCP->GetEnemyPoints());
		}
	}

	if (event.IsSameEvent("MissileFired"))
	{
		m_PlayerShots++;
	}
}

std::string PlayerScoreCP::GetRatio() const
{
	if (m_PlayerShots == 0)
		return "0";

	float ratio = (static_cast<float>(m_PlayerHits) / static_cast<float>(m_PlayerShots)) * 100.f;
	std::stringstream stream;
	stream << std::fixed << std::setprecision(2) << ratio;
	return stream.str();

}

std::string PlayerScoreCP::GetHits() const
{
	return std::to_string(m_PlayerHits);
}
std::string PlayerScoreCP::GetShots() const
{
	return std::to_string(m_PlayerShots);
}