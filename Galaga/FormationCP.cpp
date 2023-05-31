#include "FormationCP.h"
#include "GameObject.h"
#include "EnemyCP.h"
#include "AI_BeeCP.h"
#include "AI_ButterflyCP.h"
#include "HealthComponent.h"
#include "FormationReaderCP.h"
#include "TransformComponent.h"
#include "Scene.h"
#include <memory>
#include <iostream>

FormationCP::FormationCP(engine::GameObject* pOwner, const std::string& positionsJSONPath)
	:Component("FormationCP", pOwner)
	, BEES_TYPE{ "bees" }, BUTTERFLIES_TYPE{ "butterflies" }, GALAGAS_TYPE { "galagas" }
	, m_LeftLimitFormation{ -50.f }, m_pTransformCP{ nullptr }, m_MovingRight{ true }
	, m_FormationSize { 160.f }, m_FormationSpeed { 20.f }
{
	auto window = engine::SceneManager::GetInstance().GetSceneWindow();
	m_RighttLimitFormation = window.width - 300.f;
	m_pTransformCP = GetOwner()->GetComponent<engine::TransformComponent>();

	// READ JSON FILE 
	pOwner->AddComponent<FormationReaderCP>(pOwner);
	ReadFormationFromJSON(positionsJSONPath);
}

// Get all info about the formation from a JSON file
void FormationCP::ReadFormationFromJSON(const std::string& JSONPath)
{
	auto jsonReaderCP = GetOwner()->GetComponent<FormationReaderCP>();

	// Read all info about the enemies (startPos, formationPos etc) from JSON
	std::vector<std::pair<std::string, glm::vec3>> bees = jsonReaderCP->ReadFormation(JSONPath, BEES_TYPE);
	std::vector<std::pair<std::string, glm::vec3>> butterflies = jsonReaderCP->ReadFormation(JSONPath, BUTTERFLIES_TYPE);
	std::vector<std::pair<std::string, glm::vec3>> galagas = jsonReaderCP->ReadFormation(JSONPath, GALAGAS_TYPE);

	CreateEnemies(BEES_TYPE, bees);
	CreateEnemies(BUTTERFLIES_TYPE, butterflies);
	CreateEnemies(GALAGAS_TYPE, galagas);

	// Done reading everything needed
	jsonReaderCP->ClearJSONFile();

}

// Create the corresponding type of enemies
void FormationCP::CreateEnemies(const std::string& type, const std::vector< std::pair<std::string, glm::vec3>>& enemyReadInfo)
{
	// Enemies will be positioned in the formation according to its parent position
	glm::vec3 startPos{};
	glm::vec3 baseFormationPos{};
	if (m_pTransformCP != nullptr)
	{
		baseFormationPos = m_pTransformCP->GetWorldPosition();
	}
	 
	for (const auto& pair : enemyReadInfo)
	{
		auto startDirection = pair.first;
		SetStartingPos(startDirection, startPos);

		// Calculate the position of the enemy in the formation base in the current position of the base formation CP
		auto enemyPosInFormation = pair.second;
		glm::vec3 finalPosInFormation{ baseFormationPos.x + enemyPosInFormation.x, baseFormationPos.y + enemyPosInFormation.y ,
			baseFormationPos.z + enemyPosInFormation.z };

		if (type == BEES_TYPE)
		{
			CreateBee(startPos, finalPosInFormation);
		}
		else if (type == BUTTERFLIES_TYPE)
		{
			CreateButterfly(startPos, finalPosInFormation);
		}
		else
		{
			CreateGalaga(startPos, finalPosInFormation);
		}

	}

}

// Set where will the enemies will start spawning at the start of the level
void FormationCP::SetStartingPos(const std::string& commingFrom, glm::vec3& startPos)
{
	auto window = engine::SceneManager::GetInstance().GetSceneWindow();
	if (commingFrom == "top")
	{
		startPos = glm::vec3{ (window.width / 2.f ) - 20.f, -10, 0 };
	}
	else
	{
		if (commingFrom == "left")
		{
			startPos = glm::vec3{ -100, window.height / 2.f, 0 };
		}
		else if (commingFrom == "right")
		{
			startPos = glm::vec3{ window.width + 10.f , window.height / 2.f, 0 };
		}
		else
		{
			startPos = glm::vec3{ (window.width / 2.f) + 20.f, -10, 0 };
		}
	}
}

void FormationCP::CreateBee(const glm::vec3& startPos, const glm::vec3& formationPos)
{
	auto go_BeeEnemy = new engine::GameObject(GetOwner(), "Enemy", startPos, glm::vec2{ 2.f, 2.f }, false);
	go_BeeEnemy->AddComponent<EnemyCP>(go_BeeEnemy, "bee", "Sprites/Bee.png", formationPos, 1);
	go_BeeEnemy->AddComponent<AI_BeeCP>(go_BeeEnemy);
	go_BeeEnemy->GetComponent<HealthComponent>()->AddObserver(this);

	// Inactive at start
	go_BeeEnemy->SetIsActive(false);

	m_vBees.emplace_back(go_BeeEnemy);

}
void FormationCP::CreateButterfly(const glm::vec3& startPos, const glm::vec3& formationPos)
{
	auto go_Butterfly = new engine::GameObject(GetOwner(), "Enemy", startPos, glm::vec2{ 2.f, 2.f }, false);
	go_Butterfly->AddComponent<EnemyCP>(go_Butterfly, "butterfly", "Sprites/Butterfly.png", formationPos, 1);
	go_Butterfly->AddComponent<AI_ButterflyCP>(go_Butterfly);
	go_Butterfly->GetComponent<HealthComponent>()->AddObserver(this);

	// Inactive at start
	go_Butterfly->SetIsActive(false);

	m_vButterflies.emplace_back(go_Butterfly);
}

void FormationCP::CreateGalaga(const glm::vec3& startPos, const glm::vec3& formationPos)
{
	auto go_Galagas = new engine::GameObject(GetOwner(), "Enemy", startPos, glm::vec2{ 2.f, 2.f }, false);
	go_Galagas->AddComponent<EnemyCP>(go_Galagas, "galaga", "Sprites/Galaga.png", formationPos, 2);
	go_Galagas->AddComponent<AI_BeeCP>(go_Galagas);
	go_Galagas->GetComponent<HealthComponent>()->AddObserver(this);

	// Inactive at start
	go_Galagas->SetIsActive(false);

	m_vGalagas.emplace_back(go_Galagas);
}


FormationCP::~FormationCP()
{

}

void FormationCP::Update(const float deltaTime)
{
	// Move the entire formation horizontally
	if (m_pTransformCP != nullptr)
	{
		auto position = m_pTransformCP->GetWorldPosition();
		if (m_MovingRight)
		{
			if (position.x + m_FormationSize < m_RighttLimitFormation)
			{
				position.x += deltaTime * m_FormationSpeed;
				m_pTransformCP->SetLocalPosition(position);
			}
			else
			{
				m_MovingRight = false;
			}
		}
		else
		{
			if (position.x > m_LeftLimitFormation)
			{
				position.x -= deltaTime * m_FormationSpeed;
				m_pTransformCP->SetLocalPosition(position);
			}
			else
			{
				m_MovingRight = true;
			}
		}		
	}
}

void FormationCP::ReceiveMessage(const std::string& message, const std::string& value)
{
	if (message == "RemoveCP")
	{
		if (value == "TransformCP")
		{
			m_pTransformCP = nullptr;
		}
	}
}

void FormationCP::OnNotify(engine::GameObject* gameObject, const engine::Event& event)
{

	if (event.IsSameEvent("GameObjectDied"))
	{
		auto enemyCP{ gameObject->GetComponent<EnemyCP>() };
		if (enemyCP != nullptr)
		{
			if (enemyCP->GetType() == "bee")
			{
				m_BeesDeadCount++;
			}
			else if (enemyCP->GetType() == "butterfly")
			{
				m_ButterfliesDeadCount++;;
			}
			else if (enemyCP->GetType() == "galaga")
			{
				m_GalagasDeadCount++;
			}
		}
	}
}

void FormationCP::SearchForDeadEnemy()
{
	bool foundEnemy = false;
	
	for (size_t beeGOIdx{ 0 }; beeGOIdx < m_vBees.size() && !foundEnemy;)
	{
		if (m_vBees[beeGOIdx]->IsMarkedAsDead())
		{
			// Found it
			// Kill the enemy first here and then, completely destroy the gameObject shared_ptr from the scene
			// after the Update is finished. So the enemy will be actually destroyed after the update is done, and not during
			m_vBees.erase(std::remove(m_vBees.begin(), m_vBees.end(), m_vBees[beeGOIdx]), m_vBees.end());

			break;
		}
		else
		{
			beeGOIdx++;
		}
	}

	for (size_t butterflyGOIdx{ 0 }; butterflyGOIdx < m_vButterflies.size() && !foundEnemy;)
	{
		if (m_vButterflies[butterflyGOIdx]->IsMarkedAsDead())
		{
			// Found it
			m_vButterflies.erase(std::remove(m_vButterflies.begin(), m_vButterflies.end(), m_vButterflies[butterflyGOIdx]), m_vButterflies.end());
			
			break;
		}
		else
		{
			butterflyGOIdx++;
		}
	}

	for (size_t galagaGOIdx{ 0 }; galagaGOIdx < m_vGalagas.size() && !foundEnemy;)
	{
		if (m_vGalagas[galagaGOIdx]->IsMarkedAsDead())
		{
			// Found it
			m_vBees.erase(std::remove(m_vBees.begin(), m_vBees.end(), m_vBees[galagaGOIdx]), m_vBees.end());

			break;
		}
		else
		{
			galagaGOIdx++;
		}
	}


}

std::vector<engine::GameObject*>& FormationCP::GetEnemies(const std::string& type)
{
	if (type == "bees")
		return m_vBees;	
	
	if (type == "butterflies")
		return m_vButterflies;
	
	if (type == "galagas")
		return m_vGalagas;

	return m_vBees;
}

// Return true if there are no more enemies of the type specified
bool FormationCP::AreEnemiesLeft(const std::string& type) const
{
	if (type == "bees")
	{
		return m_BeesDeadCount != m_vBees.size();
	}

	if (type == "butterflies")
	{
		return m_ButterfliesDeadCount != m_vButterflies.size();
	}

	if (type == "galagas")
	{
		return m_GalagasDeadCount != m_vGalagas.size();
	}

	// Are there still enemies left?
	if (type == "all")
	{
		return m_GalagasDeadCount != m_vGalagas.size() && m_ButterfliesDeadCount != m_vButterflies.size()
			&& m_BeesDeadCount != m_vBees.size();
	}

	return false;
}

