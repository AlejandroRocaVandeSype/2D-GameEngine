#ifndef GALAGA_GAMESTATE
#define GALAGA_GAMESTATE

class GameState
{
public:

		virtual ~GameState() = default;
		virtual void OnEnter() = 0;
		virtual void OnExit() = 0;
		virtual GameState* GetChangeState() = 0;
		virtual void UpdateState(const float deltaTime) = 0;
};

#endif
