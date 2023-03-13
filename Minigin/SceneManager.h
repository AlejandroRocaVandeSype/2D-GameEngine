#ifndef GAME_ENGINE_SCENEMANAGER
#define GAME_ENGINE_SCENEMANAGER

#include <vector>
#include <string>
#include <memory>
#include "Singleton.h"
#include <cassert>

namespace dae
{
	class Scene;
	class GameObject;
	class SceneManager final : public Singleton<SceneManager>
	{
	public:
		Scene& CreateScene(const std::string& name);
		void AddToActiveScene(GameObject* gameObject);
		void Update(const float deltaTime);
		void Render();
	private:
		friend class Singleton<SceneManager>;
		SceneManager();
		std::vector<std::shared_ptr<Scene>> m_scenes;
		int m_ActiveScene;
	};
}

#endif