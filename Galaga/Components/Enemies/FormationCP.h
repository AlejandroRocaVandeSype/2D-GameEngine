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

	void Reset(const std::string& JSONPath, const std::string& formationOrderJSON);								// Reset the formation with a new one
	void DeactivateAllEnemies();

	std::vector< engine::GameObject*>& GetEnemies(const std::string& type);
	bool AreEnemiesLeft(const std::string& type="All") const;

	void SpawnEnemies();

private:

	void InitFirstFormation(const std::string& JSONPath);					// First time creating the Formation
	std::vector<std::vector<std::pair<std::string, glm::vec3>>> ReadFormationFromJSON(const std::string& JSONPath);

	// ENEMIES CREATION 
	void CreateEnemies(const std::string& type, const std::vector< std::pair<std::string, glm::vec3>>& enemyReadInfo);
	void ResetEnemies(const std::string& type, const std::vector< std::pair<std::string, glm::vec3>>& enemyReadInfo);
	void CreateBee(const glm::vec3& startPos, const glm::vec3& formationPos);
	void CreateButterfly(const glm::vec3& startPos, const glm::vec3& formationPos);
	void CreateGalaga(const glm::vec3& startPos, const glm::vec3& formationPos);

	void SetStartingPos(const std::string& commingFrom, glm::vec3& startPos);
	
	const std::string BEES_TYPE;
	const std::string BUTTERFLIES_TYPE;
	const std::string GALAGAS_TYPE;

	std::vector< engine::GameObject*> m_vBees;
	std::vector< engine::GameObject*> m_vButterflies;
	std::vector< engine::GameObject*> m_vGalagas;

	unsigned short m_BeesDeadCount;
	unsigned short m_ButterfliesDeadCount;
	unsigned short m_GalagasDeadCount;
	
	// Formation movement
	float m_FormationSize;
	const float m_FormationSpeed;
	engine::TransformComponent* m_pTransformCP;
	bool m_MovingRight;
	float m_LeftOffset;
	float m_LeftLimitFormation;
	float m_RighttLimitFormation;


};

#endif

