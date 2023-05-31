#include "AI_FormationCP.h"
#include "GameObject.h"
#include "FormationCP.h"
#include "FormationReaderCP.h"
#include "EnemyCP.h"
#include <iostream>

AI_FormationCP::AI_FormationCP(engine::GameObject* pOwner, const std::string& JSONPath)
	: Component("AI_FormationCP", pOwner)
	, m_FormationState { FormationState::spawning_enemies }, m_MovingFTState { SpawnOrderState::top_first }
	, BEES_TYPE { "bees" }, BUTTERFLIES_TYPE { "butterflies" }, GALAGAS_TYPE { "galagas" }
	, m_SpwanFirstType{true}, m_CurrentTimeSpawn{ 0.f }, m_TimeEnemySpawn { 0.5f }, m_IsSpawnInfoReaded { false }
	, m_LastEnemyType{ }, m_TimeEnemySend{ 2.5f }, NEXT_GALAGA{ 4 }, m_EnemyToSend{ BEES_TYPE }, m_SendGalagaCount {
	0
}
{
	auto jsonReaderCP = GetOwner()->GetComponent<FormationReaderCP>();
	if (jsonReaderCP != nullptr)
	{
		auto vSpawnInfo = std::move(jsonReaderCP->ReadSpawnOrder(JSONPath));		
		for (auto& enemyInfo : vSpawnInfo)
		{
			m_vSpawningInfo.emplace_back(std::make_unique<FormationReaderCP::EnemySpawnInfo>(*enemyInfo));
		}
	}

	jsonReaderCP->ClearJSONFile();
	
	m_pFormationCP = pOwner->GetComponent<FormationCP>();
}

void AI_FormationCP::GetEnemyData(const std::string& type, std::vector<engine::GameObject*>& container)
{
	auto formationCP = GetOwner()->GetComponent<FormationCP>();
	auto enemiesData = formationCP->GetEnemies(type);
	container.reserve(enemiesData.size());
	for (const auto& enemy : enemiesData)
	{
		container.emplace_back(enemy);
	}
}

AI_FormationCP::~AI_FormationCP()
{
}

void AI_FormationCP::Update(const float deltaTime)
{
	switch (m_FormationState)
	{
	case AI_FormationCP::FormationState::spawning_enemies:
		SpawnEnemies(deltaTime);
		break;
	case AI_FormationCP::FormationState::sending_enemies:
	{
		m_CurrentTimeSpawn += deltaTime;
		if (m_CurrentTimeSpawn > m_TimeEnemySend)
		{
			m_CurrentTimeSpawn = 0;
			SendEnemies();
		}
		break;
	}		
	}
}

// Activate enemies to move into the formation progresively
void AI_FormationCP::SpawnEnemies(const float deltaTime)
{
	m_CurrentTimeSpawn += deltaTime;

	if (m_CurrentTimeSpawn >= m_TimeEnemySpawn)
	{
		m_CurrentTimeSpawn = 0.f;
		
		switch (m_MovingFTState)
		{
		case AI_FormationCP::SpawnOrderState::top_first:
			UpdateSpawningBatch("top_first");
			break;
		case AI_FormationCP::SpawnOrderState::left:
		{
			// Left always will alternate between the type of enemies to be spawned
			if (!m_IsSpawnInfoReaded)
			{
				// Only get info if we haven't got it already
				for (const auto& spawnInfo : m_vSpawningInfo)
				{
					if (spawnInfo->GetStartPos() == "left")
					{
						std::pair pair = std::make_pair(spawnInfo->GetEnemyType(), spawnInfo->GetSpawnAmount());
						m_vCurrentSpawnInfo.emplace_back(pair);
					}
				}
				m_IsSpawnInfoReaded = true;
			}

			bool allEnemiesSpawned{ false };
			for (const auto& pair : m_vCurrentSpawnInfo)
			{
				// Alternate between spawning enemies
				if (m_LastEnemyType.empty() || m_LastEnemyType != pair.first)
				{
					short totalActive{ GetEnemyTypeCount(pair.first) };
					if (totalActive != -1)
					{
						m_LastEnemyType = pair.first;
						if (totalActive == pair.second)
						{
							allEnemiesSpawned = true;
						}
						else
						{
							ActivateEnemy(pair.first);
							break;
						}
					}
					else
					{
						allEnemiesSpawned = true;
					}
				}
				
			}

			if (allEnemiesSpawned)
			{
				// This batch is finished -> Activate the next batch of enemies
				m_MovingFTState = SpawnOrderState::right;
				m_IsSpawnInfoReaded = false;
				m_vCurrentSpawnInfo.clear();
			}
			break;
		}
		case AI_FormationCP::SpawnOrderState::right:
			UpdateSpawningBatch("right");
			break;		
		case AI_FormationCP::SpawnOrderState::top_second:
			UpdateSpawningBatch("top_second");
			break;
		case AI_FormationCP::SpawnOrderState::top_third:
			UpdateSpawningBatch("top_third");
			break;		
		}
	}
	
}

void AI_FormationCP::UpdateSpawningBatch(const std::string& batch)
{
	if (!m_IsSpawnInfoReaded)
	{
		// Only get info if we haven't got it already
		for (const auto& spawnInfo : m_vSpawningInfo)
		{
			if (spawnInfo->GetStartPos() == batch)
			{
				std::pair pair = std::make_pair(spawnInfo->GetEnemyType(), spawnInfo->GetSpawnAmount());
				m_vCurrentSpawnInfo.emplace_back(pair);
			}
		}
		m_IsSpawnInfoReaded = true;
	}

	bool allEnemiesSpawned{ false };
	for (const auto& pair : m_vCurrentSpawnInfo)
	{
		short totalActive{ GetEnemyTypeCount(pair.first) };
		if (totalActive != -1)
		{
			if (totalActive == pair.second)
			{
				allEnemiesSpawned = true;
			}
			else
			{
				ActivateEnemy(pair.first);
			}
		}
		else
		{
			allEnemiesSpawned = true;
		}
	}

	if (allEnemiesSpawned)
	{
		// This batch is finished -> Activate the next batch of enemies
		if (m_MovingFTState == SpawnOrderState::top_first)
		{
			m_MovingFTState = SpawnOrderState::left;
		}
		else if (m_MovingFTState == SpawnOrderState::right)
		{
			m_MovingFTState = SpawnOrderState::top_second;
		}
		else if (m_MovingFTState == SpawnOrderState::top_second)
		{
			m_MovingFTState = SpawnOrderState::top_third;
		}
		else if (m_MovingFTState == SpawnOrderState::top_third)
		{
			m_FormationState = FormationState::sending_enemies;
		}

		m_vCurrentSpawnInfo.clear();
		m_IsSpawnInfoReaded = false;
	}
}

void AI_FormationCP::SendEnemies()
{
	std::string nextEnemy = GetNextEnemyToSend();
	std::vector<engine::GameObject*> enemies = m_pFormationCP->GetEnemies(nextEnemy);
	for (auto& enemy : enemies)
	{
		if (enemy->IsActive())
		{
			EnemyCP* pEnemyCP = enemy->GetComponent<EnemyCP>();
			if (pEnemyCP->GetCurrentState() == EnemyCP::ENEMY_STATE::waiting)
			{
				// Active enemy and in waiting state -> Send him to attack
				pEnemyCP->ChangeCurrentState(EnemyCP::ENEMY_STATE::attack);
				break;
			}		
		}
	}

}

// Get the next enemy to be send taking into account if there are still enemies alive from the same type
const std::string& AI_FormationCP::GetNextEnemyToSend()
{
	// First check if it is time to Send a Galaga (every 4 enemies, one galaga is sent)
	if (m_SendGalagaCount >= NEXT_GALAGA)
	{
		if (m_pFormationCP->AreEnemiesLeft(GALAGAS_TYPE))
		{
			// Only reset if there are galagas left
			m_SendGalagaCount = 0;
			return GALAGAS_TYPE;
		}
	}
	
	// If not. Check if send bees or butterfly (If last one was bees, then now is turn of butterflies)
	if (m_EnemyToSend == BEES_TYPE)
	{
		m_SendGalagaCount++;
		if (m_pFormationCP->AreEnemiesLeft(BUTTERFLIES_TYPE))
		{
			m_EnemyToSend = BUTTERFLIES_TYPE;
		}

		if (m_pFormationCP->AreEnemiesLeft(BEES_TYPE))
		{
			return BEES_TYPE;
		}
	}
	
	if(m_EnemyToSend == BUTTERFLIES_TYPE)
	{
		m_SendGalagaCount++;
		m_EnemyToSend = BEES_TYPE;
		if (m_pFormationCP->AreEnemiesLeft(BUTTERFLIES_TYPE))
		{
			return BUTTERFLIES_TYPE;
		}
	}

	// If we reach this part means that there is no butterflies and bees left
	// so we only need to send galagas
	m_SendGalagaCount = NEXT_GALAGA;
	return GALAGAS_TYPE;
}

// Activate the corresponding enemy from the formation
void AI_FormationCP::ActivateEnemy(const std::string& type)
{
	if (m_pFormationCP != nullptr)
	{
		short currentActive = GetEnemyTypeCount(type);

		if (currentActive != -1)
		{
			std::vector<engine::GameObject*>& enemies = m_pFormationCP->GetEnemies(type);
			// Make sure we get a valid index
			if (currentActive < int(enemies.size()))
			{
				enemies[currentActive]->SetIsActive(true);
				SetEnemyTypeCount(type);
			}
		}	
	}	
}

short AI_FormationCP::GetEnemyTypeCount(const std::string& type) const
{
	if (type == BEES_TYPE)
	{
		return m_BeesActiveCount;
	}
	else if (type == BUTTERFLIES_TYPE)
	{
		return m_ButterfliesActiveCount;
	}
	else if (type == GALAGAS_TYPE)
	{
		return m_GalagasActiveCount;
	}

	// Invalid enemy type
	return -1;
}

void AI_FormationCP::SetEnemyTypeCount(const std::string& type)
{
	if (type == BEES_TYPE)
	{
		m_BeesActiveCount++;
	}
	else if (type == BUTTERFLIES_TYPE)
	{
		m_ButterfliesActiveCount++;
	}
	else if (type == GALAGAS_TYPE)
	{
		m_GalagasActiveCount++;
	}
}

void AI_FormationCP::ReceiveMessage(const std::string& message, const std::string& value)
{
	if (message == "RemoveCP")
	{
		if (value == "FormationCP")
		{
			m_pFormationCP = nullptr;
		}
	}

}