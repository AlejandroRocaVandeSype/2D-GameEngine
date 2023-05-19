#ifndef GALAGA_AI_FORMATIONCP
#define GALAGA_AI_FORMATIONCP

#include <Component.h>

// Manages all enemies from the formation, acting like a Squad leader giving orders to them
class AI_FormationCP final : public engine::Component
{

public:
	AI_FormationCP(engine::GameObject* pOwner, const std::string& JSONPath);
	~AI_FormationCP() override;

	void Update(const float deltaTime) override;
	void ReceiveMessage(const std::string&, const std::string&) override;

private:


};

#endif

