#include "Scene.h"
#include "GameObject.h"
#include "CollisionComponent.h"
#include "Servicealocator.h"
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

void Scene::AddAll(std::vector<std::shared_ptr<GameObject>>& objects)
{
	m_objects = std::move(objects);
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

	// TODO: Use spatial partitioning
	for (size_t objectIdx1{ 0 }; objectIdx1 < m_objects.size(); ++objectIdx1)
	{
		auto object1 = m_objects.at(objectIdx1);
		if (object1->IsActive())
		{
			auto collisionCP = object1->GetComponent<engine::CollisionComponent>();

			if (collisionCP)
			{
				CheckCollisions(objectIdx1, collisionCP);
			}

			// Check collisions with the children if any
			if (object1->HasChildren())
			{
				auto children = std::move(object1->GetChildren());

				int childIdx = 0;
				for (auto& child : children)
				{
					if (child->IsActive())
					{
						auto childCollisionCP = child->GetComponent<engine::CollisionComponent>();

						if (childCollisionCP)
						{
							CheckCollisions(childIdx, childCollisionCP);
						}
					}

					childIdx++;
				}
			}

		}

	}

	auto& soundSystem = Servicealocator::Get_Sound_System();
	soundSystem.ProcessRequests();
	
	// Loop again to remove dead gameObjects if any
	RemoveDeadObjects();
	
}

void Scene::CheckCollisions(size_t objectIdx1, engine::CollisionComponent* pCollisionCP)
{
	for (size_t objectIdx2{ 0 }; objectIdx2 < m_objects.size(); ++objectIdx2)
	{
		if (objectIdx2 != objectIdx1) // Dont check collisions with itself
		{
			auto object2 = m_objects.at(objectIdx2);
			if (object2->IsActive())
			{
				auto collisionCP2 = object2->GetComponent<engine::CollisionComponent>();
				if (collisionCP2)
				{
					pCollisionCP->CollisionWith(m_objects.at(objectIdx2).get(), collisionCP2);
				}

			}

		}	
	}
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

void Scene::MarkAsDeadGameObject(const engine::GameObject* pGameObjectToMark)
{
	for (auto& gameObject : m_objects)
	{
		if (gameObject.get() == pGameObjectToMark)
		{
			gameObject->MarkAsDead();
			break;
		}
	}
}

void Scene::MarkAsDeadGameObject(const std::shared_ptr<GameObject>& pGameObjectToMark)
{
	for (auto& gameObject : m_objects)
	{
		if (gameObject == pGameObjectToMark)
		{
			gameObject->MarkAsDead();
			break;
		}
	}
}

std::vector<std::shared_ptr<engine::GameObject>>& Scene::GetAll()
{
	return m_objects;
}

engine::GameObject* Scene::FindGameObjectByTag(const std::string& tag, bool IsActive)
{
	for (auto& gameObject : m_objects)
	{
		if (gameObject->Tag() == tag)
		{
			if (IsActive)
			{
				// Return only if Active
				if (gameObject->IsActive())
				{
					return gameObject.get();
				}
				else
				{
					return nullptr;
				}
			}
			else
			{
				// Doenst matter if it is active or not.
				return gameObject.get();
			}		
		}
	}
	// GameObject with that tag not found
	return nullptr;
}

const std::string& Scene::Name() const
{
	return m_name;
}


