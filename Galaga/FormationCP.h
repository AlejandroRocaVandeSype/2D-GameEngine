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


// CONTAINS ALL ENEMIES FROM THE FORMATION.
// enemies will move with the formation when they are in the formation
class FormationCP final : public engine::Component, public engine::Observer
{
public:
	FormationCP(engine::GameObject* pOwner, const std::string& JSONPath);
	~FormationCP() override;

	void Update(const float deltaTime) override;
	void ReceiveMessage(const std::string&, const std::string&) override;

	void OnNotify(engine::GameObject* gameObject, const engine::Event& event) override;

	void ReadFormationFromJSON(const std::string& JSONPath);

	std::vector< engine::GameObject*>& GetEnemies(const std::string& type);


private:

	// ENEMIES CREATION 
	void CreateEnemies(const std::string& type, const std::vector< std::pair<std::string, glm::vec3>>& enemyReadInfo);
	void CreateBee(const glm::vec3& startPos, const glm::vec3& formationPos);
	void CreateButterfly(const glm::vec3& startPos, const glm::vec3& formationPos);
	void CreateGalaga(const glm::vec3& startPos, const glm::vec3& formationPos);

	void SetStartingPos(const std::string& commingFrom, glm::vec3& startPos);
	void SearchForDeadEnemy();
	
	const std::string BEES_TYPE;
	const std::string BUTTERFLIES_TYPE;
	const std::string GALAGAS_TYPE;

	std::vector< engine::GameObject*> m_vBees;
	std::vector< engine::GameObject*> m_vButterflies;
	std::vector< engine::GameObject*> m_vGalagas;
	
	// Formation movement
	const float m_FormationSize;
	const float m_FormationSpeed;
	engine::TransformComponent* m_pTransformCP;
	bool m_MovingRight;
	float m_LeftLimitFormation;
	float m_RighttLimitFormation;


};

#endif

