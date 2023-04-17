#include "LivesUIComponent.h"
#include "GameObject.h"
#include "Event.h"
#include "RenderComponent.h"

LivesUIComponent::LivesUIComponent(dae::GameObject* pOwner, glm::vec2 positions)
	:Component("LivesUICP", pOwner), Observer()
{

	auto renderCP{ pOwner->GetComponent<dae::RenderComponent>() };
	glm::vec2 textureSize;
	if (renderCP != nullptr)
	{
		textureSize = renderCP->GetTextureSize();

		m_vUILivesPos.push_back(positions);

		for (int i{ 0 }; i < 2; ++i)
		{
			positions.x += textureSize.x + 5.f;
			m_vUILivesPos.push_back(positions);
		}

		renderCP->SetPositionsToRender(m_vUILivesPos);
	}
	

}


LivesUIComponent::~LivesUIComponent()
{

}

void LivesUIComponent::Update([[maybe_unused]] const float deltaTime)
{

}

void LivesUIComponent::ReceiveMessage([[maybe_unused]] const std::string& message, [[maybe_unused]] const std::string& value)
{

}

void LivesUIComponent::OnNotify(dae::GameObject* gameObject, const Event& event)
{
	if (event.IsSameEvent("HealthDecremented"))
	{
		if (!m_vUILivesPos.empty())
		{
			m_vUILivesPos.pop_back();
			auto renderCP{ gameObject->GetComponent<dae::RenderComponent>()};

			if (renderCP != nullptr)
			{
				renderCP->SetPositionsToRender(m_vUILivesPos);
			}

		}
		
	}
}