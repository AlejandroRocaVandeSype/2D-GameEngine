#ifndef LIVESUI_COMPONENT
#define LIVESUI_COMPONENT

#include "Component.h"
#include "Observer.h"
#include <glm/glm.hpp>
#include <vector>

class LivesUIComponent final : public Component, public Observer
{
public:
	LivesUIComponent(dae::GameObject* pOwner, const std::string& spriteFileName, glm::vec2 livesPos, unsigned int amountLives);
	~LivesUIComponent() override;

	virtual void Update(const float deltaTime) override;
	virtual void ReceiveMessage(const std::string& message, const std::string& value) override;

	void OnNotify(dae::GameObject* gameObject, const Event& event) override;

private:
	std::vector<dae::GameObject*> m_vUILives;			// Contains GameObjects that represents the lives 

};

#endif

