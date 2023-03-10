#include "FPSComponent.h"
#include "TextComponent.h"
#include "GameObject.h"
#include <iostream>

using namespace dae;

FPSComponent::FPSComponent(GameObject* pOwner)
	: Component("FPSCP", pOwner)
	, m_FpsTimer { 0 }
	, m_FrameCount { 0 }
	, MAX_SECOND { 1.0f }
{
	if (pOwner != nullptr)
	{
		m_pTextCP = pOwner->GetComponent<TextComponent>();
	}
	else
	{
		m_pTextCP = nullptr;
	}
}

FPSComponent::~FPSComponent()
{
	std::cout << "FPSComponent destructor" << std::endl;
}

void FPSComponent::Update(const float deltaTime)
{

	if (m_FpsTimer >= MAX_SECOND)
	{
		// We get how many frames we get in one second
		int fps{ int(m_FrameCount / m_FpsTimer) };
		std::string fpsString = std::to_string(fps) + " FPS";
		
		if (m_pTextCP != nullptr)
		{
			// Send the new fps string value to display
			m_pTextCP->SetText(fpsString);
		}
		m_FrameCount = 0;
		m_FpsTimer = 0;

	}

	// Inc frame counter
	m_FrameCount++;
	m_FpsTimer += deltaTime;

}

void FPSComponent::ReceiveMessage(const std::string& message, const std::string& value)
{
	if (message == "RemoveCP")
	{
		if (value == "TextCP")
		{
			m_pTextCP = nullptr;
		}
	}
}