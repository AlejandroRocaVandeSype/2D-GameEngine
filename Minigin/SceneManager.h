#ifndef GAME_ENGINE_SCENEMANAGER
#define GAME_ENGINE_SCENEMANAGER

#include <vector>
#include <string>
#include <memory>
#include "Singleton.h"
#include <cassert>
#include "structs.h"

namespace engine
{
	class Scene;
	class GameObject;
	class SceneManager final : public Singleton<SceneManager>
	{
	public:
		Scene& CreateScene(const std::string& sceneName);

		void Update(const float deltaTime);
		void Render();

		// Adding GameObjects to scenes
		void AddToActiveScene(GameObject* gameObject);
		void AddToActiveScene(std::shared_ptr<GameObject> gameObject);
		void AddToScene(const std::string& sceneName, std::shared_ptr<GameObject> object);

		engine::GameObject* FindGameObjectByTag(const std::string& tag);

		void SetActiveScene(const std::string& sceneName);
		Scene& GetActiveScene();

		void SetSceneWindow(const engine::Window& window);
		const engine::Window& GetSceneWindow() const;
		
	private:
		friend class Singleton<SceneManager>;
		SceneManager();

		std::vector<std::shared_ptr<Scene>> m_scenes;
		const int INVALID_SCENE;
		int m_ActiveScene;
		
		engine::Window m_Window;
	};
}

#endif