#include "GameOverState.h"
#include "glm/vec3.hpp"
#include "SceneManager.h"
#include "GameObject.h"
#include "ResourceManager.h"
#include "TextComponent.h"
#include "RenderComponent.h"
#include "Scene.h"
#include <iostream>

GameOverState::~GameOverState()
{

}

void GameOverState::OnEnter()
{
	std::cout << "GameOver State" << std::endl;

	auto& sceneManager = engine::SceneManager::GetInstance();
	auto& scene = sceneManager.GetActiveScene();
	auto& window = sceneManager.GetSceneWindow();

	glm::vec3 gameOverPos{ (window.width/2.f) - 80.f, (window.height/2.f) - 80.f, 0.f };
	auto galaga_Font = engine::ResourceManager::GetInstance().LoadFont("Fonts/Emulogic-zrEw.ttf", 18);
	SDL_Color blueColor = { 0,255, 222 };

	auto gameOverText{ std::make_shared<engine::GameObject>(nullptr, "UI", gameOverPos) };
	gameOverText->AddComponent<engine::RenderComponent>(gameOverText.get());
	gameOverText->AddComponent<TextComponent>(gameOverText.get(), "GAME OVER", galaga_Font, blueColor);

	scene.Add(gameOverText);


}
void GameOverState::OnExit()
{

}

GameState* GameOverState::GetChangeState()
{
	return nullptr;
}
void GameOverState::UpdateState(const float )
{

}