#include "Scene.h"
#include "GameObject.h"
#include "CollisionComponent.h"
#include <iostream>

using namespace engine;

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
		std::cerr << " Error! Trying to add a gameObject that already has parent to the scene" << '\n';
	}
	
}

void Scene::Render() const
{
	for (const auto& object : m_objects)
	{
		object->Render();
	}
}

void Scene::Update(const float deltaTime)
{
	
	for(auto& object : m_objects)
	{
		object->Update(deltaTime);
	}

	// TODO: Use space partitioning
	// Check collisions between the gameObjects that have collisionCP
	for (size_t objectIdx1{ 0 }; objectIdx1 < m_objects.size(); ++objectIdx1)
	{
		auto object = m_objects.at(objectIdx1);
		if (object->IsActive())
		{
			auto collisionCP = m_objects.at(objectIdx1)->GetComponent<engine::CollisionComponent>();

			if (collisionCP)
			{
				for (size_t objectIdx2{ 0 }; objectIdx2 < m_objects.size(); ++objectIdx2)
				{
					auto collisionCP2 = m_objects.at(objectIdx2)->GetComponent<engine::CollisionComponent>();
					if (collisionCP2 && objectIdx2 != objectIdx1)
					{
						collisionCP->CollisionWith(m_objects.at(objectIdx2).get(), collisionCP2);
					}
				}
			}
		}

	}
	
	// Loop again to remove dead gameObjects if any
	RemoveDeadObjects();
	
}

// Remove all "dead" objects after update
void Scene::RemoveDeadObjects()
{
	for (size_t objectIdx{ 0 }; objectIdx < m_objects.size();)
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
				m_objects[objectIdx]->DeleteDeadChildren();
			}

			objectIdx++;
		}
	}
}

void Scene::Remove(std::shared_ptr<GameObject> object)
{
	m_objects.erase(std::remove(m_objects.begin(), m_objects.end(), object), m_objects.end());
}


void Scene::RemoveAll()
{
	m_objects.clear();
}



