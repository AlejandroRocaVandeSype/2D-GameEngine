#pragma once
#include "Component.h"
#include <glm/glm.hpp>


class TransformComponent final : public Component
{

public:

	TransformComponent(dae::GameObject* pOwner, glm::vec3 position = glm::vec3{0.f, 0.f, 0.f}, float rotation = 0.0f, 
		glm::vec3 scale = glm::vec3{ 1.f, 1.f, 1.f });
	~TransformComponent();
	virtual void Update(const float deltaTime) override;
	virtual void ReceiveMessage(const std::string& message, const std::string& value) override;

	void UpdateWorldPosition();

	const glm::vec3 GetWorldPosition();
	const glm::vec3 GetLocalPosition() const;

	void SetLocalPosition(const glm::vec3& position);
	void SetPositionDirty();

private:
	glm::vec3 m_WorldPosition;		// Global position relative with the world space
	glm::vec3 m_LocalPosition;		// Position relative to the parent (local space) If no parent localPos = worldPos
	glm::vec3 m_ScaleVector;
	float m_Rotation;				// In radians

	bool m_IsPositionDirty;

};

