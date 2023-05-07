#pragma once
#include <Component.h>
#include "Observer.h"

class EnemyCP final: public engine::Component, public engine::Observer
{
	public:
		EnemyCP(engine::GameObject* pOwner, unsigned int health);
		virtual ~EnemyCP() override;

		virtual void Update(const float deltaTime) override;
		virtual void ReceiveMessage(const std::string& message, const std::string& value) override;

		void OnNotify(engine::GameObject* gameObject, const engine::Event& event) override;

};

