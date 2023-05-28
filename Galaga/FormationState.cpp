#include "FormationState.h"
#include "SpawningState.h"
#include "SendingEnemiesState.h"

SpawningState* FormationState::spawning = new SpawningState();
SendingEnemiesState* FormationState::sendingEnemies = new SendingEnemiesState();