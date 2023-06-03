#ifndef GALAGA_MENUSTATE
#define GALAGA_MENUSTATE

#include "GameState.h"
#include <memory>
#include <vector>
#include "GameObject.h"


class MenuState final : public GameState
{
public:
	
	~MenuState() override;
	void OnEnter() override;
	void OnExit() override;
	GameState* GetChangeState() override;
	void UpdateState(const float deltaTime) override;

	void StartGame();


private:

	void InitBackground();
	void InitMenuInput();
	void InitMenuUI();
	std::vector<std::shared_ptr <engine::GameObject>> m_vBackgrounds;
	std::shared_ptr <engine::GameObject> m_Title;

	bool m_ChangeState = false;

};
#endif
