#ifndef GALAGA_ENEMYCP
#define GALAGA_ENEMYCP
#include <Component.h>
#include "Observer.h"
#include "glm/vec3.hpp"

class EnemyCP final: public engine::Component, public engine::Observer
{
	public:
		EnemyCP(engine::GameObject* pOwner, const glm::vec3 formationPos, unsigned int health);
		virtual ~EnemyCP() override;

		virtual void Update(const float deltaTime) override;
		virtual void ReceiveMessage(const std::string& message, const std::string& value) override;

		void OnNotify(engine::GameObject* gameObject, const engine::Event& event) override;

		glm::vec3 GetFormationPos() const;

private:
	glm::vec3 m_FormationPos;
};

#endif

