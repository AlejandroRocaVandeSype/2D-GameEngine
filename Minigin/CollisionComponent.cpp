#include "CollisionComponent.h"
#include "GameObject.h"
#include <iostream>
#include "Event.h"

engine::CollisionComponent::CollisionComponent(engine::GameObject* pOwner, const glm::vec2& gameObjectSize)
	:Component("CollisionCP", pOwner)
	, m_Size { gameObjectSize }
{
	
	auto pTransformCP = pOwner->GetComponent<engine::TransformComponent>();
	if (pTransformCP != nullptr)
	{
		auto gameObjectPos = pTransformCP->GetWorldPosition();
		SetBoundingBox(gameObjectPos);
		// This way the transformCP will update boundingBox position when the GO move
		pTransformCP->AddCollisionCP(this);   
	}

}


engine::CollisionComponent::~CollisionComponent()
{

}

void engine::CollisionComponent::AddObserver(engine::Observer* pObserver)
{
	if (m_CollisionSubject == nullptr)
	{
		m_CollisionSubject = std::make_unique<engine::Subject>();
	}

	m_CollisionSubject->AddObserver(pObserver);
}

void engine::CollisionComponent::CollisionWith(engine::GameObject* other, engine::CollisionComponent* otherCollisionCP)
{
	// Only check if there is an observer interested on it and if there are not the same type
	if (m_CollisionSubject != nullptr && other->Tag() != GetOwner()->Tag())
	{
		engine::Rectf otherBoundingBox = otherCollisionCP->GetBoundingBox();

		if (m_BoundingBox.left < otherBoundingBox.width && m_BoundingBox.width > otherBoundingBox.left)
		{
			if (m_BoundingBox.bottom < otherBoundingBox.height && m_BoundingBox.height > otherBoundingBox.bottom)
			{
				// Both gameObjects are colliding -> Sent a notification to anyone who is interested
				engine::Event collisionEvent{ "CollisionWith " + other->Tag()};
				m_CollisionSubject->NotifyObservers(other, collisionEvent);
			}
		}
	}
	
}

engine::Rectf engine::CollisionComponent::GetBoundingBox() const
{
	return m_BoundingBox;
}

// Updates the boundingBox with the gameObject position
void engine::CollisionComponent::SetBoundingBox(const glm::vec3& gameObjectPos)
{
	m_BoundingBox.left = gameObjectPos.x;
	m_BoundingBox.bottom = gameObjectPos.y;
	m_BoundingBox.width = m_BoundingBox.left + m_Size.x;
	m_BoundingBox.height = m_BoundingBox.bottom + m_Size.y;
}

void engine::CollisionComponent::Update([[maybe_unused]] const float deltaTime)
{

}
void engine::CollisionComponent::ReceiveMessage([[maybe_unused]] const std::string& message, [[maybe_unused]] const std::string& value)
{

}