#include "EnemyCP.h"
#include "GameObject.h"
#include "CollisionComponent.h"
#include "HealthComponent.h"
#include <iostream>

EnemyCP::EnemyCP(engine::GameObject* pOwner, unsigned int health)
	: Component("EnemyCP", pOwner)
{
	if (pOwner != nullptr)
	{
		pOwner->AddComponent<engine::RenderComponent>(pOwner, "Enemy.png");	
		pOwner->AddComponent<HealthComponent>(pOwner, health);
		pOwner->AddComponent<engine::CollisionComponent>(pOwner, pOwner->GetComponent<engine::RenderComponent>()->GetTextureSize());
		pOwner->GetComponent<engine::CollisionComponent>()->AddObserver(this);
	}
}

EnemyCP::~EnemyCP()
{

}

void EnemyCP::Update([[maybe_unused]] const float deltaTime)
{

}
void EnemyCP::ReceiveMessage([[maybe_unused]] const std::string& message, [[maybe_unused]] const std::string& value)
{

}

void EnemyCP::OnNotify([[maybe_unused]] engine::GameObject* gameObject, const engine::Event& event)
{
	if (event.IsSameEvent("CollisionWith Player") || event.IsSameEvent("CollisionWith PlayerMissile"))
	{
		auto healthCP = GetOwner()->GetComponent<HealthComponent>();
		if (healthCP != nullptr)
		{
			healthCP->DecrementHealth(1);
		}
	}
}