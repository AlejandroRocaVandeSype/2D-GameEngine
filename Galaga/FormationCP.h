#ifndef GALAGA_FORMATIONCP
#define GALAGA_FORMATIONCP
#include <Component.h>
#include <vector>
#include <glm/vec3.hpp>

// Component that represents all positions in the formation for the enemies
class FormationCP final : public engine::Component
{
public:
	FormationCP(engine::GameObject* pOwner, const std::string& formationJsonPath);
	~FormationCP() override;

	void Update(const float deltaTime) override;
	void ReceiveMessage(const std::string& message, const std::string& value) override;

	void ReadFormationFromJSON(const std::string& formationJsonPath);

	std::vector<glm::vec3> GetFormation(const std::string& enemyType) const;

private:
	std::vector<glm::vec3> m_vBeesPos;
	std::vector<glm::vec3> m_vButterfliesPos;
	std::vector<glm::vec3> m_vGalagasPos;

};

#endif

