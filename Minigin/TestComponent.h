#ifndef GAME_ENGINE_TESTCOMPONENT
#define GAME_ENGINE_TESTCOMPONENT

#include "Component.h"
#include <glm/glm.hpp>

class TransformComponent;
class TestComponent : public Component
{
public:
	TestComponent(dae::GameObject* pOwner);
	~TestComponent();
	virtual void Update(const float deltaTime) override;
	virtual void ReceiveMessage(const std::string& message, const std::string& value) override;

private:

	TransformComponent* m_pTransformCP{};
	bool m_IsDone{ false };

};

#endif
