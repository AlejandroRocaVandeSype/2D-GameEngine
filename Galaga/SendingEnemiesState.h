#ifndef GALAGA_SENDINGENEMIES_STATE
#define GALAGA_SENDINGENEMIES_STATE
#include "FormationState.h"

class SendingEnemiesState final : public FormationState
{
public:
	SendingEnemiesState();
	void UpdateState(const float deltaTime) override;
	FormationState* GetNewState() override;

};

#endif
