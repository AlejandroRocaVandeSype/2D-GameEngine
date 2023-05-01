#ifndef GAME_ENGINE_TESTCOMPONENT
#define GAME_ENGINE_TESTCOMPONENT

#include "Component.h"
#include <glm/glm.hpp>

namespace engine
{
	class TransformComponent;
}

class TestComponent : public engine::Component
{
public:
	TestComponent(engine::GameObject* pOwner);
	~TestComponent() override;
	virtual void Update(const float deltaTime) override;
	virtual void ReceiveMessage(const std::string& message, const std::string& value) override;

private:

	engine::TransformComponent* m_pTransformCP{};
	bool m_IsDone{ false };

};

#endif
