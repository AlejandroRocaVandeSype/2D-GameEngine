#include "TestComponent.h"
#include "TransformComponent.h"
#include "GameObject.h"
#include <iostream>

TestComponent::TestComponent(engine::GameObject* pOwner)
	:Component("TestCP", pOwner)
{
	if (pOwner != nullptr)
	{
		m_pTransformCP = pOwner->GetComponent<engine::TransformComponent>();
	}
}

TestComponent::~TestComponent()
{
	std::cout << "TestCP destructor" << std::endl;
}

void TestComponent::ReceiveMessage(const std::string& message, const std::string& value)
{
	if (message == "RemoveCP")
	{
		if (value == "TransformCP")
		{
			m_pTransformCP = nullptr;
		}
	}
}

void TestComponent::Update(const float deltaTime)
{
	if (m_pTransformCP != nullptr)
	{
		glm::vec3 pos = m_pTransformCP->GetLocalPosition();

		pos.x += 20.f * deltaTime;

		m_pTransformCP->SetLocalPosition(pos);

		if (pos.x >= 400.f && m_IsDone == false)
		{
			GetOwner()->RemoveParentFromChildren();
			GetOwner()->MarkAsDead();
			m_IsDone = true;
		}

	}
}
