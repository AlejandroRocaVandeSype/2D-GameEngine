#include "SceneManager.h"
#include "Scene.h"
#include "GameObject.h"
#include <iostream>

engine::SceneManager::SceneManager()
	: INVALID_SCENE { -1 }
	, m_ActiveScene { INVALID_SCENE }
{
	m_Window = engine::Window{ "DefaultWindow", 100.f , 100.f };
}

void engine::SceneManager::Update(const float deltaTime)
{
	if (!m_scenes.empty() && m_ActiveScene != -1)
	{
		m_scenes.at(m_ActiveScene)->Update(deltaTime);
	}
	/*
	for(auto& scene : m_scenes)
	{
		scene->Update(deltaTime);
	}
	*/
}

void engine::SceneManager::Render()
{
	if (!m_scenes.empty() && m_ActiveScene != -1)
	{
		m_scenes.at(m_ActiveScene)->Render();
	}
	/*
	for (const auto& scene : m_scenes)
	{
		scene->Render();
	}
	*/
}

engine::Scene& engine::SceneManager::CreateScene(const std::string& name)
{
	const auto& scene = std::shared_ptr<Scene>(new Scene(name));
	m_scenes.push_back(scene);

	if (m_ActiveScene == -1)
	{
		// First scene created
		m_ActiveScene = 0;
	}
	
	return *scene;
}

void engine::SceneManager::AddToActiveScene(GameObject* gameObject)
{
	if (!gameObject->getParent())
	{
		// We dont want the scene to own gameObjects that are already owned by their parents
		if ( m_ActiveScene >=0 && m_ActiveScene < (int)m_scenes.size())
		{
			// There is an active scene --> Add the gameObject to it
			std::shared_ptr<GameObject> sharedPtr(gameObject);
			m_scenes.at(m_ActiveScene)->Add(sharedPtr);
		}
	}
	else
	{
		std::cerr << "Error! Trying to add a gameObject that already has parent to the scene" << '\n';
	}
	
}

void engine::SceneManager::AddToActiveScene(std::shared_ptr<GameObject> object)
{
	
	if (m_ActiveScene >= 0 && m_ActiveScene < (int)m_scenes.size())
	{
		// There is an active scene --> Add the gameObject to it
		m_scenes.at(m_ActiveScene)->Add(object);
	}
	
}

void engine::SceneManager::AddToScene(const std::string& sceneName, std::shared_ptr<GameObject> object)
{
	for (auto& scene : m_scenes)
	{
		if (scene->Name() == sceneName)
		{
			scene->Add(object);
			break;
		}
	}

}

engine::GameObject* engine::SceneManager::FindGameObjectByTag(const std::string& tag)
{
	if(m_ActiveScene >= 0 && m_ActiveScene < (int)m_scenes.size())
	{
		return m_scenes.at(m_ActiveScene)->FindGameObjectByTag(tag);
	}
	
	return nullptr;
}

void engine::SceneManager::SetActiveScene(const std::string& sceneName)
{
	for (size_t sceneIdx{ 0 }; sceneIdx < m_scenes.size(); ++sceneIdx)
	{
		if (m_scenes.at(sceneIdx)->Name() == sceneName)
		{
			m_ActiveScene = int(sceneIdx);
			break;
		}
	}
}

void engine::SceneManager::SetSceneWindow(const engine::Window& window)
{
	m_Window = window;
}

const engine::Window& engine::SceneManager::GetSceneWindow() const
{
	return m_Window;
}

engine::Scene& engine::SceneManager::GetActiveScene()
{
	if (!m_scenes.empty() && m_ActiveScene != -1)
	{
		return *m_scenes.at(m_ActiveScene);
	}

	// If no scene yet --> Create a default one
	return CreateScene("defaultScene");
	
}
