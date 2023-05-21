#ifndef GALAGA_AI_FORMATIONCP
#define GALAGA_AI_FORMATIONCP

#include <Component.h>
#include <vector>

// Manages all enemies from the formation, acting like a Squad leader giving orders to them
class AI_FormationCP final : public engine::Component
{

public:
	AI_FormationCP(engine::GameObject* pOwner, const std::string& JSONPath);
	~AI_FormationCP() override;

	void Update(const float deltaTime) override;
	void ReceiveMessage(const std::string&, const std::string&) override;

private:

	enum class FormationState
	{
		setupFormation,
		attacking
	};

	enum class MovingFormationState
	{
		top,
		left, 
		right,
		backToTop
	};

	void GetEnemyData(const std::string& type, std::vector<engine::GameObject*>& container);
	void MoveIntoFormation(const float deltaTime);
	void ActivateEnemy(std::vector<engine::GameObject*> enemies, unsigned short& currentActive);


	FormationState m_FormationState;
	MovingFormationState m_MovingFTState;

	// SETUP FORMATION DATA
	unsigned short m_TopFormationAmount;
	unsigned short m_LeftFormationAmount;
	unsigned short m_Top1FormationAmount;
	unsigned short m_RightFormationAmount;
	unsigned short m_BeesActive;
	unsigned short m_ButterfliesActive;
	unsigned short m_GalagasActive;
	bool m_SpwanFirstType;				// For the left batch of enemies.

	std::vector<engine::GameObject*> m_vBees;
	std::vector<engine::GameObject*> m_vButterflies;
	std::vector<engine::GameObject*> m_vGalagas;

	const float m_TimeEnemySpawn;
	float m_CurrentTimeSpawn;
};

#endif

