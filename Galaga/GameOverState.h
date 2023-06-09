#ifndef GALAGA_GAMEOVERSTATE
#define GALAGA_GAMEOVERSTATE

#include "GameState.h"
#include <vector>

namespace engine
{
	class GameObject;
}
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

	void UpdateUIObjects(const float deltaTime);
	float m_elapsedSec{ 0.f };
	float m_MaxTime{ 1.5f };

	std::vector<engine::GameObject*> m_GameOverObjects{};
	int m_CurrentShowing{ 0 };

};

#endif

