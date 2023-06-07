#ifndef GALAGA_ENEMYCP
#define GALAGA_ENEMYCP
#include <Component.h>
#include "Observer.h"
#include "glm/vec2.hpp"
#include "glm/vec3.hpp"

namespace engine
{
	class TransformComponent;
}
class MoveComponent;
class MissileManagerCP;
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
		void CalculateMissileDirection();

		void Reset(const glm::vec3& startPos, const glm::vec3& formationPos);

		glm::vec3 GetFormationPos() const;
		ENEMY_STATE GetCurrentState() const;
		const std::string& GetType() const;
	

private:

	void UpdateMoveToFormation(const float deltaTime);

	// Firing Missiles - ALL enemies type can fire
	void FireMissile(const float deltaTime);

	glm::vec3 m_FormationPos;				// Position the enemy will be placed in the formation
	ENEMY_STATE m_CurrentState;
	std::string m_EnemyType;
	engine::TransformComponent* m_pTransformCP;
	MoveComponent* m_pMoveCP;
	MissileManagerCP* m_pMissileManagerCP;

	// Firing Behaviour
	glm::vec3 m_MissileDir;
	bool m_HasShoot;
	unsigned short m_AmountMissiles;			// How many missiles he will be able to fire
	unsigned short m_MissilesShoot;				// How many missiles he has already fired
	float m_ElapsedShootTime;
	float m_WaitBetweenShoot;					// How much time to wait between one missile and another
};

#endif

