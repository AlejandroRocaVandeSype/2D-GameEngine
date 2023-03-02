#pragma once
#include "Component.h"
#include <glm/glm.hpp>

class TransformComponent final : public Component
{

public:

	TransformComponent(glm::vec3 position = glm::vec3{0.f, 0.f, 0.f});
	TransformComponent(float x, float y, float z);
	~TransformComponent();
	virtual void Update([[maybe_unused]] const float deltaTime) override;
	virtual void ReceiveMessage(const std::string& message, const std::string& value) override;

	const glm::vec3& GetPosition() const { return m_position; }
	void SetPosition(float x, float y, float z);
	void SetPosition(glm::vec3 position);

private:
	glm::vec3 m_position;


};

