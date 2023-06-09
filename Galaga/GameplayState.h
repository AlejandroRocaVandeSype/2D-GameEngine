#ifndef GALAGA_GAMEPLAYSTATE
#define GALAGA_GAMEPLAYSTATE

#include "GameState.h"
#include "structs.h"

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

	void InitEnemies();
	void InitPlayer1();
	void InitPlayer2();

	// Reset everything need it for the new Stage
	void Reset();

	// GAMEOBJECT TAGS
	

	const std::string m_GameMode;

	FormationCP* m_pFormationCP;
	bool change = true;
};

#endif
