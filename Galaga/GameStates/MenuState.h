#ifndef GALAGA_MENUSTATE
#define GALAGA_MENUSTATE

#include "GameState.h"
#include <memory>
#include <vector>
#include "GameObject.h"

class MenuSelectionCP;
class MenuState final : public GameState
{
public:
	MenuState() = default;
	~MenuState() override;
	void OnEnter() override;
	void OnExit() override;
	GameState* GetChangeState() override;
	void UpdateState(const float deltaTime) override;

	void SwitchToMenuOptions();

private:

	void InitBackground();
	void InitMenuInput();
	void InitUI();

	std::vector< std::shared_ptr <engine::GameObject>> m_vMenuGO;			// All gameObjects from the Menu state
	MenuSelectionCP* m_pMenuSelectionCP{};
	engine::GameObject* m_pControlsImage{};

	const std::string ONE_PLAYER_OPT{ "1 PLAYER" };
	const std::string TWO_PLAYERS_OPT{ "2 PLAYERS" };

	bool m_ChangeState = false;

};
#endif
