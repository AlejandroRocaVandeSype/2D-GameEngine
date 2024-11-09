#ifndef GALAGA_PARALLAX_SCROLLING_CP
#define GALAGA_PARALLAX_SCROLLING_CP

#include "Component.h"
#include <glm/vec3.hpp>
#include <glm/vec2.hpp>

namespace engine
{
	class GameObject;
	class TransformComponent;
}

// Simple Parallax effect for the Background image
class ParallaxScrollingCP final : public engine::Component
{
public:
	ParallaxScrollingCP(engine::GameObject* pOwner);
	virtual ~ParallaxScrollingCP() override;

	virtual void Update(const float deltaTime) override;
	virtual void ReceiveMessage(const std::string& message, const std::string& value) override;

	void ActivateScrolling();

private:
	engine::TransformComponent* m_pTransformCP;
	glm::vec2 m_TextureSize;
	const float SPEED;
	const glm::vec3 m_Direction;   // Parallax movement direction
	bool m_IsScrollingActive;		// To know if background has to move or not (deactivated at start)
};

#endif