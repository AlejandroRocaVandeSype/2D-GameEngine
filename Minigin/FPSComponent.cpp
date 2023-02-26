#include "FPSComponent.h"
#include "TextComponent.h"
#include <string>

using namespace dae;

FPSComponent::FPSComponent(TextComponent* pTextCP)
	: Component(Component::FPSCP)
	, m_FpsTimer { 0 }
	, m_FrameCount { 0 }
	, MAX_SECOND { 1.0f }
	, m_pTextCP { pTextCP }
{

}


void FPSComponent::Update([[maybe_unused]] const float deltaTime)
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