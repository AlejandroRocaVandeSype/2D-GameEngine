#include "Scene.h"
#include "GameObject.h"
#include <iostream>

using namespace dae;

unsigned int Scene::m_idCounter = 0;

Scene::Scene(const std::string& name) : m_name(name) {}

Scene::~Scene() = default;

void Scene::Add(std::shared_ptr<GameObject> object)
{
	if (!object->getParent())
	{
		m_objects.emplace_back(std::move(object));
	}
	else
	{
		std::cerr << " Error! Trying to add a gameObject that already has parent to the scene" << std::endl;
	}
	
	
	
	
}

void Scene::Remove(std::shared_ptr<GameObject> object)
{
	m_objects.erase(std::remove(m_objects.begin(), m_objects.end(), object), m_objects.end());
}

// Remove all "dead" objects after update
void Scene::RemoveDeadObjects()
{
	for (size_t objectIdx { 0 }; objectIdx < m_objects.size();)
	{
		if (m_objects[objectIdx]->IsMarkedAsDead())
		{			
			// If removed dont increase the itr
			// If parent is removed, no need to check children because they will be destoyed too
			Remove(m_objects[objectIdx]);
		}
		else
		{
			if (m_objects[objectIdx]->HasChildren())
			{
				m_objects[objectIdx]->RemoveDeadChildren();
			}

			objectIdx++;
		}
	}
}

void Scene::RemoveAll()
{
	m_objects.clear();
}

void Scene::Update(const float deltaTime)
{
	for(auto& object : m_objects)
	{
		object->Update(deltaTime);
	}
	
	// Loop again to remove dead gameObjects if any
	RemoveDeadObjects();
	
}

void Scene::Render() const
{
	for (const auto& object : m_objects)
	{
		object->Render();
	}
}

