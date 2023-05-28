#ifndef GALAGA_FORMATION_STATE
#define GALAGA_FORMATION_STATE

#include <memory>

class SpawningState;
class SendingEnemiesState;
class FormationState
{
public:
	
	static SpawningState* spawning;
	static SendingEnemiesState* sendingEnemies;
	virtual void UpdateState(const float deltaTime) = 0;
	virtual FormationState* GetNewState() = 0;

};


#endif

