#ifndef GAME_ENGINE_TRANSFORMCOMPONENT
#define GAME_ENGINE_TRANSFORMCOMPONENT

#include "Component.h"
#include <glm/vec3.hpp>
#include <glm/vec2.hpp>

namespace engine
{
	class CollisionComponent;
	class TransformComponent final : public Component
	{

	public:

		TransformComponent(engine::GameObject* pOwner, glm::vec3 position = glm::vec3{ 0.f, 0.f, 0.f },
			glm::vec2 scale = glm::vec2{ 1.f, 1.f });
		virtual ~TransformComponent() override;
		virtual void Update(const float deltaTime) override;
		virtual void ReceiveMessage(const std::string& message, const std::string& value) override;

		void UpdateWorldPosition();

		const glm::vec3 GetWorldPosition();
		const glm::vec3 GetLocalPosition() const;
		const glm::vec2 GetScale() const;

		void SetLocalPosition(const glm::vec3& position);
		void SetPositionDirty();
		void SetScale(const glm::vec2& scale);

		void AddCollisionCP(engine::CollisionComponent* pComponent);

	private:
		glm::vec3 m_WorldPosition;		// Global position relative with the world space
		glm::vec3 m_LocalPosition;		// Position relative to the parent (local space) If no parent localPos = worldPos
		glm::vec2 m_Scale;

		bool m_IsPositionDirty;
		engine::CollisionComponent* m_pCollisionCP;			// To update the boundingBox position so it moves with the Gameobject
		bool m_MoveWithParent;
	};
}
#endif