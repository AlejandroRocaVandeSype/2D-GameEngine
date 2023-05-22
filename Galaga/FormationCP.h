#ifndef GALAGA_FORMATIONCP
#define GALAGA_FORMATIONCP
#include <Component.h>
#include <vector>
#include <memory>
#include "glm/vec3.hpp"
#include "Observer.h"

namespace engine
{
	class TransformComponent;
}

// Component that will contain all enemies from the current level and will
// init them with correct info and add them to the scene
class FormationCP final : public engine::Component, public engine::Observer
{
public:
	FormationCP(engine::GameObject* pOwner, const std::string& JSONPath);
	~FormationCP() override;

	void Update(const float deltaTime) override;
	void ReceiveMessage(const std::string&, const std::string&) override;

	void OnNotify(engine::GameObject* gameObject, const engine::Event& event) override;

	void ReadFormationFromJSON(const std::string& JSONPath);

	std::vector< engine::GameObject*> GetEnemies(const std::string& type);


private:

	void SetStartingPos(const std::string& commingFrom, glm::vec3& startPos);
	void SearchForDeadEnemy();
	
	std::vector< engine::GameObject*> m_vBees;
	std::vector< engine::GameObject*> m_vButterflies;
	std::vector< engine::GameObject*> m_vGalagas;

	int beesPos = 0;
	float timeBees = 1.f;
	
	// Formation movement
	const float m_FormationSize;
	const float m_FormationSpeed;
	engine::TransformComponent* m_pTransformCP;
	bool m_MovingRight;
	float m_LeftLimitFormation;
	float m_RighttLimitFormation;

};

#endif

