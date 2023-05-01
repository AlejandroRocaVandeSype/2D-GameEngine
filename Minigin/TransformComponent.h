#ifndef GAME_ENGINE_TRANSFORMCOMPONENT
#define GAME_ENGINE_TRANSFORMCOMPONENT

#include "Component.h"
#include <glm/glm.hpp>
namespace engine
{
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
		glm::vec3 GetWorldPositionRelative() const;
		const glm::vec3 GetLocalPosition() const;
		const glm::vec2 GetScale() const;

		void SetLocalPosition(const glm::vec3& position);
		void SetCenterOffset(const glm::vec3& centerOffset);
		void SetPositionDirty();
		void SetScale(const glm::vec2& scale);

	private:
		glm::vec3 m_WorldPosition;		// Global position relative with the world space
		glm::vec3 m_LocalPosition;		// Position relative to the parent (local space) If no parent localPos = worldPos
		glm::vec2 m_Scale;
		glm::vec3 m_CenterOffset;		// For making the object rotate around its own center when no parent
		//float m_Rotation;				// In radians

		bool m_IsPositionDirty;

	};
}
#endif