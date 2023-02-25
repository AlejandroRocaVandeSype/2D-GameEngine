#include <stdexcept>
#include <SDL_ttf.h>
#include "TextObject.h"
#include "Renderer.h"
#include "Font.h"
#include "Texture2D.h"
#include "TextComponent.h"

dae::TextObject::TextObject(const std::string& text, std::shared_ptr<Font> font, glm::vec3 position)
	//: m_needsUpdate(true), m_text(text), m_font(std::move(font)), m_textTexture(nullptr)
{
	m_GameObject = std::make_shared<dae::GameObject>(); // Unique ptr?
	m_pRenderCP = m_GameObject->AddComponent<TextComponent>();
	
	if (m_pRenderCP != nullptr)
	{
		m_pRenderCP->SetText(text);
		m_pRenderCP->SetFont(std::move(font));
	}

	if (position.x != 0 || position.y != 0)
	{
		// Not default position at (0,0) set the new Position
		auto t = m_GameObject->GetComponent<TransformComponent>();
		t->SetPosition(position);
	}

}

void dae::TextObject::Update([[maybe_unused]] const float deltaTime)
{
	/*
	if (m_needsUpdate)
	{
		/*
		const SDL_Color color = { 255,255,255 }; // only white text is supported now
		const auto surf = TTF_RenderText_Blended(m_font->GetFont(), m_text.c_str(), color);
		if (surf == nullptr) 
		{
			throw std::runtime_error(std::string("Render text failed: ") + SDL_GetError());
		}
		auto texture = SDL_CreateTextureFromSurface(Renderer::GetInstance().GetSDLRenderer(), surf);
		if (texture == nullptr) 
		{
			throw std::runtime_error(std::string("Create text texture from surface failed: ") + SDL_GetError());
		}
		SDL_FreeSurface(surf);
		m_textTexture = std::make_shared<Texture2D>(texture);
		*/
		//m_pRenderCP->CreateTextureFromText(m_font, m_text); // Update from render?
		//m_needsUpdate = false;
	//}

}

std::shared_ptr<dae::GameObject> dae::TextObject::GameObject() const
{
	return m_GameObject;
}

/*
void dae::TextObject::Render() const
{
	
	if (m_textTexture != nullptr)
	{
		const auto& pos = m_transform.GetPosition();
		Renderer::GetInstance().RenderTexture(*m_textTexture, pos.x, pos.y);
	}
}
*/

// This implementation uses the "dirty flag" pattern
void dae::TextObject::SetText(const std::string& text)
{
	if (m_pRenderCP != nullptr)
	{
		m_pRenderCP->SetText(text);
	}
	/*
	m_text = text;
	m_needsUpdate = true;
	*/
}

/*
void dae::TextObject::SetPosition(const float x, const float y)
{
	m_transform.SetPosition(x, y, 0.0f);
}
*/

