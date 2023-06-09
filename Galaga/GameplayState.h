#ifndef GALAGA_GAMEPLAYSTATE
#define GALAGA_GAMEPLAYSTATE

#include "GameState.h"
#include "structs.h"
#include <vector>

class FormationCP;
class GameplayState final : public GameState
{
public:

	GameplayState(const std::string& gameMode);
	~GameplayState() override;
	void OnEnter() override;
	void OnExit() override;
	GameState* GetChangeState() override;
	void UpdateState(const float deltaTime) override;

private:

	// Struct that contains data for the Formations of enemies
	struct FormationJsonData 
	{
	public:
		FormationJsonData(const std::string& enemiesJson, const std::string& spawnOrderJson)
			:m_EnemiesData{ enemiesJson }, m_SpwanOrderData{ spawnOrderJson } {}

		const std::string& GetEnemiesData() const { return m_EnemiesData;  }
		const std::string& GetSpawnOrderData() const { return m_SpwanOrderData; }

	private:
		const std::string m_EnemiesData;				// Info with all enemies and their positions at the formation
		const std::string m_SpwanOrderData;				// Order in which enemies will appear
	};

	void InitEnemies();
	void InitPlayer1();
	void InitPlayer2();

	bool NextStage();
	
	const std::string m_GameMode;
	FormationCP* m_pFormationCP;
	std::vector<std::pair<std::string, FormationJsonData>> m_vEnemiesData;

	bool m_IsGameOver;


};

#endif
