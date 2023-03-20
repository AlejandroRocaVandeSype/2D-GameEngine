#ifndef GAME_ENGINE_GAME
#define GAME_ENGINE_GAME

#include <memory>
#include "structs.h"


namespace dae
{
	class Renderer;
	class SceneManager;
	class Scene;
	class GameObject;
	class Game final
	{
	public:

		Game(const Window& window);
		~Game();
		Game(const Game& other) = delete;
		Game(Game&& other) = delete;
		Game& operator=(const Game& other) = delete;
		Game& operator=(Game&& other) = delete;

		void Update(const float deltaTime);
		void Render() const;

	private:

		const Window m_Window;
		Renderer* m_pRenderer{};
		SceneManager* m_pSceneManager{};
		Scene* m_pScene{};

	};
}

#endif

