#include "AI_FormationCP.h"
#include "GameObject.h"
#include "FormationCP.h"
#include "FormationReaderCP.h"
#include <iostream>

AI_FormationCP::AI_FormationCP(engine::GameObject* pOwner, const std::string& JSONPath)
	: Component("AI_FormationCP", pOwner)
	, m_FormationState { FormationState::spawning }, m_MovingFTState { SpawnOrderState::top_first }
	, m_SpwanFirstType{true}, m_CurrentTimeSpawn{ 0.f }, m_TimeEnemySpawn { 0.5f }, m_IsSpawnInfoReaded { false }
	, m_LastEnemyType { }
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
	case AI_FormationCP::FormationState::spawning:
		SpawnEnemies(deltaTime);
		break;
	case AI_FormationCP::FormationState::attacking:
		break;

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
		{		
			if (!m_IsSpawnInfoReaded)
			{
				// Only get info if we haven't got it already
				for (const auto& spawnInfo : m_vSpawningInfo)
				{
					if (spawnInfo->GetStartPos() == "top_first")
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
				m_MovingFTState = SpawnOrderState::left;
				m_vCurrentSpawnInfo.clear();
				m_IsSpawnInfoReaded = false;
			}

			break;
		}
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
		{
			if (!m_IsSpawnInfoReaded)
			{
				// Only get info if we haven't got it already
				for (const auto& spawnInfo : m_vSpawningInfo)
				{
					if (spawnInfo->GetStartPos() == "right")
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
				m_MovingFTState = SpawnOrderState::top_second;
				m_vCurrentSpawnInfo.clear();
				m_IsSpawnInfoReaded = false;
			}
			break;
		}			
		case AI_FormationCP::SpawnOrderState::top_second:
		{
			if (!m_IsSpawnInfoReaded)
			{
				// Only get info if we haven't got it already
				for (const auto& spawnInfo : m_vSpawningInfo)
				{
					if (spawnInfo->GetStartPos() == "top_second")
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
				m_MovingFTState = SpawnOrderState::top_third;
				m_vCurrentSpawnInfo.clear();
				m_IsSpawnInfoReaded = false;
			}
			break;
		}
		case AI_FormationCP::SpawnOrderState::top_third:
		{
			if (!m_IsSpawnInfoReaded)
			{
				// Only get info if we haven't got it already
				for (const auto& spawnInfo : m_vSpawningInfo)
				{
					if (spawnInfo->GetStartPos() == "top_third")
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
				
				m_FormationState = FormationState::attacking;
				m_vCurrentSpawnInfo.clear();
				m_IsSpawnInfoReaded = false;
			}
			break;
		}
			
		}
	}
	

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
	if (type == "bees")
	{
		return m_BeesActiveCount;
	}
	else if (type == "butterflies")
	{
		return m_ButterfliesActiveCount;
	}
	else if (type == "galagas")
	{
		return m_GalagasActiveCount;
	}

	// Invalid enemy type
	return -1;
}

void AI_FormationCP::SetEnemyTypeCount(const std::string& type)
{
	if (type == "bees")
	{
		m_BeesActiveCount++;
	}
	else if (type == "butterflies")
	{
		m_ButterfliesActiveCount++;
	}
	else if (type == "galagas")
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