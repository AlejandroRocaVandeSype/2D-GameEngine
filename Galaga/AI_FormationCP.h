#ifndef GALAGA_AI_FORMATIONCP
#define GALAGA_AI_FORMATIONCP

#include <Component.h>
#include <vector>
#include "FormationReaderCP.h"

class FormationCP;

// Manages all enemies from the formation, acting like a Squad leader giving orders to them
class AI_FormationCP final : public engine::Component
{

public:

	AI_FormationCP(engine::GameObject* pOwner, const std::string& JSONPath);
	~AI_FormationCP() override;

	void Update(const float deltaTime) override;
	void ReceiveMessage(const std::string&, const std::string&) override;

private:

	enum class FormationState
	{
		spawning,
		attacking
	};

	enum class SpawnOrderState
	{
		top_first,
		left, 
		right,
		top_second,
		top_third
	};

	void GetEnemyData(const std::string& type, std::vector<engine::GameObject*>& container);
	void SpawnEnemies(const float deltaTime);
	void ActivateEnemy(const std::string& type);
	short GetEnemyTypeCount(const std::string& type) const;
	void SetEnemyTypeCount(const std::string& type);

	FormationCP* m_pFormationCP;
	FormationState m_FormationState;
	SpawnOrderState m_MovingFTState;

	// SPAWNING ENEMIES 
	std::vector<std::unique_ptr<FormationReaderCP::EnemySpawnInfo>> m_vSpawningInfo; // All spawn info for all enemies 
	std::vector < std::pair<std::string, short>> m_vCurrentSpawnInfo;				// Info from the current batch spawning
	bool m_IsSpawnInfoReaded;
	std::string m_LastEnemyType;							// For the left batch of enemies.
	short m_BeesActiveCount;
	short m_ButterfliesActiveCount;
	short m_GalagasActiveCount;
	bool m_SpwanFirstType;				

	

	const float m_TimeEnemySpawn;
	float m_CurrentTimeSpawn;
};

#endif

