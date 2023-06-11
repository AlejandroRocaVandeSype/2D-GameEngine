#ifndef GAME_ENGINE_COLLISIONCOMPONENT
#define GAME_ENGINE_COLLISIONCOMPONENT

#include "Component.h"
#include <memory>
#include "Subject.h"
#include "structs.h"		// For Rectf
#include <glm/vec3.hpp>
#include <glm/vec2.hpp>

namespace engine
{
	class Observer;
	// Provides a simple collision handling to the GameObject
	class CollisionComponent final : public engine::Component
	{
	public:
		CollisionComponent(engine::GameObject* pOwner, const glm::vec2& gameObjectSize);

		virtual ~CollisionComponent() override;

		virtual void Update(const float deltaTime) override;
		virtual void ReceiveMessage(const std::string& message, const std::string& value) override;

		void CollisionWith(engine::GameObject* other, engine::CollisionComponent* otherCollisionCP);
		void AddObserver(engine::Observer* pObserver);

		engine::Rectf GetBoundingBox() const;
		void SetBoundingBox(const glm::vec3& gameObjectPos);

	private:
		std::unique_ptr<engine::Subject> m_CollisionSubject;
		Rectf m_BoundingBox;								// Used to check collisions
		glm::vec2 m_Size;									// Size of the GameObject
	};

}

#endif
