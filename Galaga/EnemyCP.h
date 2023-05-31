#ifndef GALAGA_ENEMYCP
#define GALAGA_ENEMYCP
#include <Component.h>
#include "Observer.h"
#include "glm/vec2.hpp"
#include "glm/vec3.hpp"

class EnemyCP final: public engine::Component, public engine::Observer
{
	public:
		// ALL ENEMIES WILL HAVE THESE 3 STATES, but attack will be different for each enemy
		enum class ENEMY_STATE
		{
			waiting,					// Waits in the formation until he receives the order to attack
			moveToFormation,			
			attack
		};

		EnemyCP(engine::GameObject* pOwner, const std::string& spriteFilePath, const std::string& enemyType,
			const glm::vec3 formationPos, unsigned int health);
		virtual ~EnemyCP() override;

		virtual void Update(const float deltaTime) override;
		virtual void ReceiveMessage(const std::string& message, const std::string& value) override;

		void OnNotify(engine::GameObject* gameObject, const engine::Event& event) override;

		void ChangeCurrentState(EnemyCP::ENEMY_STATE newState);

		glm::vec3 GetFormationPos() const;
		ENEMY_STATE GetCurrentState() const;
		const std::string& GetType() const;
	

private:
	glm::vec3 m_FormationPos;				// Position the enemy will be placed in the formation
	ENEMY_STATE m_CurrentState;
	std::string m_EnemyType;
};

#endif

