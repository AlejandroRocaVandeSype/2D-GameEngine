#ifndef GALAGA_SPAWNING_STATE
#define GALAGA_SPAWNING_STATE

#include "FormationState.h"
class SpawningState final : public FormationState
{
public:

	 SpawningState();
	 void UpdateState(const float deltaTime) override;
	 FormationState* GetNewState() override;

private:
};

#endif
