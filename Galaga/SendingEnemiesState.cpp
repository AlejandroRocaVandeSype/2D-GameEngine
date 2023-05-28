#include "SendingEnemiesState.h"
#include "SpawningState.h"

SendingEnemiesState::SendingEnemiesState()
{

}

void SendingEnemiesState::UpdateState(const float)
{

}

FormationState* SendingEnemiesState::GetNewState()
{
	return FormationState::spawning;
}