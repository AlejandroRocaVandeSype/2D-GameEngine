#include "PlayerScoreCP.h"
#include "GameObject.h"
#include "Component.h"
#include "TextComponent.h"
#include "SceneManager.h"
#include "ResourceManager.h"
#include <iostream>

PlayerScoreCP::PlayerScoreCP(engine::GameObject* pOwner, unsigned int playerIdx)
	: Component("PlayerScoreCP", pOwner)
{
	auto& sceneManager = engine::SceneManager::GetInstance();
	auto& window = sceneManager.GetSceneWindow();

	glm::vec3 scorePos{ 0.f, 20.f, 0.f };
	glm::vec3 playerSymbolPos{ 0.f, 0.f, 0.f };
	std::string playerSymbol{ };

	if (playerIdx == 1)
	{
		// Left side of the screen
		scorePos.x = 20.f;
		playerSymbolPos.x = 80.f;
		playerSymbol = "1UP";
	}
	else
	{
		// Right side of the screen
		scorePos.x = window.width - 150.f;
		playerSymbolPos.x = window.width - 90.f;
		playerSymbol = "2UP";
	}

	auto galaga_Font = engine::ResourceManager::GetInstance().LoadFont("Fonts/Emulogic-zrEw.ttf", 17);
	engine::GameObject* pScore = new engine::GameObject(pOwner, std::string{ "UI" }, scorePos);
	pScore->AddComponent<engine::RenderComponent>(pScore);
	pScore->AddComponent<TextComponent>(pScore, "0000000", galaga_Font);


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
void PlayerScoreCP::ReceiveMessage(const std::string& , const std::string& )
{

}

void PlayerScoreCP::OnNotify(engine::GameObject* , const engine::Event& event)
{
	if (event.IsSameEvent("UpdatePoints") )
	{
		std::cout << "Entro" << std::endl;
	}
}