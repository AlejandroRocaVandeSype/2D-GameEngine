#ifndef GALAGA_GAMEPLAYSTATE
#define GALAGA_GAMEPLAYSTATE

#include "GameState.h"
#include "structs.h"
#include <vector>
#include <memory>

class FormationCP;
namespace engine
{
	class GameObject;
}
class GameplayState final : public GameState
{
public:

	GameplayState(const std::string& gameMode);
	~GameplayState() override;
	void OnEnter() override;
	void OnExit() override;
	GameState* GetChangeState() override;
	void UpdateState(const float deltaTime) override;
	bool ArePlayersAlive();
	void SkipStage();

private:

	// Struct that contains data for the Formations of enemies
	struct FormationJsonData 
	{
	public:
		FormationJsonData(const std::string& enemiesJson, const std::string& spawnOrderJson)
			:m_EnemiesData{ enemiesJson }, m_SpwanOrderData{ spawnOrderJson } {}

		const std::string& GetEnemiesData() const { return m_EnemiesData;  }
		const std::string& GetSpawnOrderData() const { return m_SpwanOrderData; }
		void changeData(const std::string& enemiesJson, const std::string& spawnOrderJson)
		{
			m_EnemiesData = enemiesJson;
			m_SpwanOrderData = spawnOrderJson;
		}

	private:
		std::string m_EnemiesData;				// Info with all enemies and their positions at the formation
		std::string m_SpwanOrderData;				// Order in which enemies will appear
	};

	void LoadDataPaths();
	void InitEnemies();
	void InitPlayer1();
	void InitPlayer2();

	bool NextStage();
	void UpdateChangingStage(const float deltaTime);

	
	const std::string m_GameMode;
	FormationCP* m_pFormationCP;
	std::vector<std::pair<std::string, FormationJsonData>> m_vEnemiesData;
	std::shared_ptr<engine::GameObject> m_StageInfoText;

	std::vector<engine::GameObject*> m_vPlayers;		// Keep track of the player(s) to see if they die

	bool m_IsGameOver;
	bool m_IsChangingState;
	float m_ElapsedTime;
	const float MAX_TIME_CHANGE;						// Wait a few seconds until loading enemies from the stage
	std::string m_CurrentStage;

};

#endif
