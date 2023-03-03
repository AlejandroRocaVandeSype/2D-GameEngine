#include <stdexcept>
#include "TextObject.h"
#include "Renderer.h"
#include "Font.h"
#include "Texture2D.h"
#include "TextComponent.h"
#include "RenderComponent.h"
#include "FPSComponent.h"

dae::TextObject::TextObject([[maybe_unused]] const std::string& text, [[maybe_unused]] std::shared_ptr<Font> font, glm::vec3 position, [[maybe_unused]] const SDL_Color& color)
{
	m_GameObject = std::make_shared<dae::GameObject>(); 
	//m_pRenderCP = m_GameObject->AddComponent<RenderComponent>();
	//m_pTextCP = m_GameObject->AddComponent<TextComponent>(text, font, m_pRenderCP, color);

	if (position.x != 0 || position.y != 0)
	{
		// Not default position at (0,0) set the new Position
		auto t = m_GameObject->GetComponent<TransformComponent>();
		t->SetPosition(position);
	}

}

void dae::TextObject::Update([[maybe_unused]] const float deltaTime)
{
	

}

std::shared_ptr<dae::GameObject> dae::TextObject::GameObject() const
{
	return m_GameObject;
}




// This implementation uses the "dirty flag" pattern
void dae::TextObject::SetText(const std::string& text)
{
	
	if (m_pTextCP != nullptr)
	{
		m_pTextCP->SetText(text);
	}
	
}

void dae::TextObject::AddFPSComponent()
{
	//m_GameObject->AddComponent<FPSComponent>(m_pTextCP);
}