#ifndef GALAGA_GAMEOVERSTATE
#define GALAGA_GAMEOVERSTATE

#include "GameState.h"

class GameOverState final : public GameState
{
public:
	GameOverState() = default;
	~GameOverState() override;

	void OnEnter() override;
	void OnExit() override;

	GameState* GetChangeState() override;
	void UpdateState(const float deltaTime) override;

private:


};

#endif

