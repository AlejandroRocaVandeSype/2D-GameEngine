#ifndef LIVESUI_COMPONENT
#define LIVESUI_COMPONENT

#include "Component.h"
#include "Observer.h"
#include <glm/glm.hpp>
#include <vector>

class LivesUIComponent final : public Component, public Observer
{
public:
	LivesUIComponent(dae::GameObject* pOwner, glm::vec2 positions);
	~LivesUIComponent() override;

	virtual void Update(const float deltaTime) override;
	virtual void ReceiveMessage(const std::string& message, const std::string& value) override;

	void OnNotify(dae::GameObject* gameObject, const Event& event) override;

private:
	std::vector<glm::vec2> m_vUILivesPos;  // Positions where the lives will be shown

};

#endif

