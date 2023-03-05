#pragma once

#include <memory>

namespace dae
{
	class Renderer;
	class SceneManager;
	class Scene;
	class GameObject;
	class Game final
	{
	public:

		Game();
		~Game();
		Game(const Game& other) = delete;
		Game(Game&& other) = delete;
		Game& operator=(const Game& other) = delete;
		Game& operator=(Game&& other) = delete;

		void Update(const float deltaTime);
		void Render() const;

	private:

		Renderer* m_pRenderer{};
		SceneManager* m_pSceneManager{};
		Scene* m_pScene{};
		
	};
}


