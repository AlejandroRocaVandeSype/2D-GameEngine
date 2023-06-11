#ifndef LIVESUI_COMPONENT
#define LIVESUI_COMPONENT

#include "Component.h"
#include "Observer.h"
#include <glm/vec2.hpp>
#include <vector>

class LivesUIComponent final : public engine::Component, public engine::Observer
{
public:
	LivesUIComponent(engine::GameObject* pOwner, const std::string& spriteFileName, glm::vec2 livesPos, unsigned int amountLives);
	~LivesUIComponent() override;

	virtual void Update(const float deltaTime) override;
	virtual void ReceiveMessage(const std::string& message, const std::string& value) override;

	void OnNotify(engine::GameObject* gameObject, const engine::Event& event) override;

private:
	std::vector<engine::GameObject*> m_vUILives;			// Contains GameObjects that represents the lives 

};

#endif

