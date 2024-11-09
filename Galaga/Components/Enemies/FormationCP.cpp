#include "FormationCP.h"
#include "../../Galaga_Strings.h"
#include "GameObject.h"
#include "EnemyCP.h"
#include "AI_BeeCP.h"
#include "AI_ButterflyCP.h"
#include "AI_GalagaCP.h"
#include "AI_FormationCP.h"
#include "../General/HealthComponent.h"
#include "FormationReaderCP.h"
#include "TransformComponent.h"
#include "Scene.h"
#include <memory>

FormationCP::FormationCP(engine::GameObject* pOwner, const std::string& positionsJSONPath)
	:Component("FormationCP", pOwner)
	, BEES_TYPE{ "bees" }, BUTTERFLIES_TYPE{ "butterflies" }, GALAGAS_TYPE { "galagas" }
	, m_LeftLimitFormation{ 0.f }, m_pTransformCP{ nullptr }, m_MovingRight{ true }
	, m_FormationSize { 500.f }, m_FormationSpeed { 20.f }, m_LeftOffset { 50.f }
{
	auto window = engine::SceneManager::GetInstance().GetSceneWindow();
	m_RighttLimitFormation = window.width;
	m_pTransformCP = GetOwner()->GetComponent<engine::TransformComponent>();

	// READ JSON FILE and Init formation
	pOwner->AddComponent<FormationReaderCP>(pOwner);
	InitFirstFormation(positionsJSONPath);
}

// First time creating the formation -> Enemies need to be fully created
void FormationCP::InitFirstFormation(const std::string& JSONPath)
{
	auto vEnemies = std::move(ReadFormationFromJSON(JSONPath));
	if (vEnemies.size() > 0 && vEnemies.size() < 4)
	{
		CreateEnemies(BEES_TYPE, vEnemies.at(0));
		CreateEnemies(BUTTERFLIES_TYPE,vEnemies.at(1));
		CreateEnemies(GALAGAS_TYPE, vEnemies.at(2));
	}
	
}

// Get all info about the formation from a JSON file
std::vector< std::vector<std::pair<std::string, glm::vec3>>> FormationCP::ReadFormationFromJSON(const std::string& JSONPath)
{
	std::vector< std::vector<std::pair<std::string, glm::vec3>>> vEnemies;

	auto jsonReaderCP = GetOwner()->GetComponent<FormationReaderCP>();
	if (jsonReaderCP != nullptr)
	{		
		// Read all info about the enemies (startPos, formationPos etc) from JSON
		std::vector<std::pair<std::string, glm::vec3>> bees = jsonReaderCP->ReadFormation(JSONPath, BEES_TYPE);
		std::vector<std::pair<std::string, glm::vec3>> butterflies = jsonReaderCP->ReadFormation(JSONPath, BUTTERFLIES_TYPE);
		std::vector<std::pair<std::string, glm::vec3>> galagas = jsonReaderCP->ReadFormation(JSONPath, GALAGAS_TYPE);

		vEnemies.emplace_back(bees);
		vEnemies.emplace_back(butterflies);
		vEnemies.emplace_back(galagas);

		// Done reading everything needed
		jsonReaderCP->ClearJSONFile();
	}
	
	return vEnemies;
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
	go_BeeEnemy->AddComponent<EnemyCP>(go_BeeEnemy, STR_BEE, "Sprites/Enemies/bees.png", formationPos, 1);
	go_BeeEnemy->AddComponent<AI_BeeCP>(go_BeeEnemy);
	go_BeeEnemy->GetComponent<HealthComponent>()->AddObserver(this);
	
	// Inactive at start
	go_BeeEnemy->SetIsActive(false);

	m_vBees.emplace_back(go_BeeEnemy);

}
void FormationCP::CreateButterfly(const glm::vec3& startPos, const glm::vec3& formationPos)
{
	auto go_Butterfly = new engine::GameObject(GetOwner(), "Enemy", startPos, glm::vec2{ 2.f, 2.f }, false);
	go_Butterfly->AddComponent<EnemyCP>(go_Butterfly, STR_BUTTERFLY, "Sprites/Enemies/butterflies.png", formationPos, 1);
	go_Butterfly->AddComponent<AI_ButterflyCP>(go_Butterfly);
	go_Butterfly->GetComponent<HealthComponent>()->AddObserver(this);

	// Inactive at start
	go_Butterfly->SetIsActive(false);

	m_vButterflies.emplace_back(go_Butterfly);
}

void FormationCP::CreateGalaga(const glm::vec3& startPos, const glm::vec3& formationPos)
{
	auto go_Galagas = new engine::GameObject(GetOwner(), "Enemy", startPos, glm::vec2{ 2.f, 2.f }, false);
	go_Galagas->AddComponent<EnemyCP>(go_Galagas, STR_GALAGA, GALAGA_SPRITE_V1, formationPos, 2);
	go_Galagas->AddComponent<AI_GalagaCP>(go_Galagas);
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
			if (position.x + m_LeftOffset > m_LeftLimitFormation)
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
			if (enemyCP->GetType() == STR_BEE)
			{
				m_BeesDeadCount++;
			}
			else if (enemyCP->GetType() == STR_BUTTERFLY)
			{
				m_ButterfliesDeadCount++;;
			}
			else if (enemyCP->GetType() ==STR_GALAGA)
			{
				m_GalagasDeadCount++;
			}
		}
	}
}


void FormationCP::Reset(const std::string& JSONPath, const std::string& formationOrderJSON)
{
	if (m_pTransformCP != nullptr)
	{
		m_pTransformCP->SetLocalPosition(glm::vec3{ 0.f, 0.f, 0.f });
		m_MovingRight = true;
		m_FormationSize = 550.f;
		m_LeftOffset = 60.f;
	}

	// Read new json file with the formation
	auto vEnemies = std::move(ReadFormationFromJSON(JSONPath));	
	if (vEnemies.size() > 0 && vEnemies.size() < 4)
	{
		ResetEnemies(BEES_TYPE, vEnemies.at(0));
		ResetEnemies(BUTTERFLIES_TYPE, vEnemies.at(1));
		ResetEnemies(GALAGAS_TYPE, vEnemies.at(2));


	}

	auto AI_formation = GetOwner()->GetComponent<AI_FormationCP>();
	if (AI_formation != nullptr)
	{
		AI_formation->Reset(formationOrderJSON);
	}


	
}

void FormationCP::ResetEnemies(const std::string& type, const std::vector< std::pair<std::string, glm::vec3>>& enemyReadInfo)
{
	glm::vec3 startPos{};
	glm::vec3 baseFormationPos{};
	if (m_pTransformCP != nullptr)
	{
		baseFormationPos = m_pTransformCP->GetWorldPosition();
	}

	for (size_t enemyIdx{ 0 }; enemyIdx < enemyReadInfo.size(); ++enemyIdx)
	{
		auto startDirection = enemyReadInfo[enemyIdx].first;
		SetStartingPos(startDirection, startPos);

		// Calculate the position of the enemy in the formation base in the current position of the base formation CP
		auto enemyPosInFormation = enemyReadInfo[enemyIdx].second;
		glm::vec3 finalPosInFormation{ baseFormationPos.x + enemyPosInFormation.x, baseFormationPos.y + enemyPosInFormation.y ,
			baseFormationPos.z + enemyPosInFormation.z };

		if (type == BEES_TYPE)
		{
			if (enemyIdx < m_vBees.size())
			{
				auto enemyCP{ m_vBees[enemyIdx]->GetComponent<EnemyCP>() };
				if (enemyCP != nullptr)
				{
					enemyCP->Reset(startPos, finalPosInFormation);
				}
			}
		}
		else if (type == BUTTERFLIES_TYPE)
		{
			if (enemyIdx < m_vButterflies.size())
			{
				auto enemyCP{ m_vButterflies[enemyIdx]->GetComponent<EnemyCP>() };
				if (enemyCP != nullptr)
				{
					enemyCP->Reset(startPos, finalPosInFormation);
				}
			}
		}
		else
		{
			if (enemyIdx < m_vGalagas.size())
			{
				auto enemyCP{ m_vGalagas[enemyIdx]->GetComponent<EnemyCP>() };
				if (enemyCP != nullptr)
				{
					enemyCP->Reset(startPos, finalPosInFormation);
				}
			}
		}
	}

	m_GalagasDeadCount = m_ButterfliesDeadCount = m_BeesDeadCount = 0;
	
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

void FormationCP::DeactivateAllEnemies()
{
	for (const auto& bee : m_vBees)
	{
		bee->SetIsActive(false);
	}

	for (const auto& butterfly : m_vButterflies)
	{
		butterfly->SetIsActive(false);
	}

	for (const auto& galaga : m_vGalagas)
	{
		galaga->SetIsActive(false);
	}

	auto aiFormation = GetOwner()->GetComponent<AI_FormationCP>();
	if (aiFormation != nullptr)
	{
		aiFormation->ChangeToWaitState();
	}

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
	if (type == "All")
	{
		if (m_GalagasDeadCount != m_vGalagas.size())
			return true;
		if (m_ButterfliesDeadCount != m_vButterflies.size())
			return true;
		if (m_BeesDeadCount != m_vBees.size())
			return true;
		// No enemies left
		return false;
	}

	return false;
}

void FormationCP::SpawnEnemies()
{
	auto aiFormation = GetOwner()->GetComponent<AI_FormationCP>();
	if (aiFormation != nullptr)
	{
		aiFormation->SpawnEnemies();
	}
}

