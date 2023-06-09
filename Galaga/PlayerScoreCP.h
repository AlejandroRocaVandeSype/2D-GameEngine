#pragma once
#include <Component.h>
#include "Observer.h"
#include "glm/vec2.hpp"

class PlayerScoreCP final : public engine::Component, public engine::Observer
{
public:
	PlayerScoreCP(engine::GameObject* pOwner, unsigned int playerIdx);
	~PlayerScoreCP() override;

	virtual void Update(const float deltaTime) override;
	virtual void ReceiveMessage(const std::string& message, const std::string& value) override;

	void OnNotify(engine::GameObject* gameObject, const engine::Event& event) override;

private:

};

