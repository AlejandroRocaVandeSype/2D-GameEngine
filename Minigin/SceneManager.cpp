#include "SceneManager.h"
#include "Scene.h"
#include "GameObject.h"
#include <iostream>

engine::SceneManager::SceneManager()
	: m_ActiveScene { -1 }
{}

void engine::SceneManager::Update(const float deltaTime)
{
	for(auto& scene : m_scenes)
	{
		scene->Update(deltaTime);
	}
}

void engine::SceneManager::Render()
{
	for (const auto& scene : m_scenes)
	{
		scene->Render();
	}
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
