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
		Scene& CreateScene(const std::string& name);
		void AddToActiveScene(GameObject* gameObject);
		void AddToActiveScene(std::shared_ptr<GameObject> gameObject);
		void Update(const float deltaTime);
		void Render();
		
	private:
		friend class Singleton<SceneManager>;
		SceneManager();
		std::vector<std::shared_ptr<Scene>> m_scenes;
		int m_ActiveScene;
		const engine::Window* m_Window;
	};
}

#endif