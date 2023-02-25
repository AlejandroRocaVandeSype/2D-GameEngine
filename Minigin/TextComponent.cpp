#include "TextComponent.h"
#include "Font.h"
#include "Texture2D.h"
#include "Renderer.h"
#include <stdexcept>
#include <SDL_ttf.h>

using namespace dae;

TextComponent::TextComponent()
	: RenderComponent()
{

}

void TextComponent::Update([[maybe_unused]] const float deltaTime)
{
	if (m_needsUpdate)
	{
		RenderComponent::Update(deltaTime);
		CreateTextureFromText();
		m_needsUpdate = false;
	}
}

void TextComponent::CreateTextureFromText()
{
	const SDL_Color color = { 255,255,255 }; // only white text is supported now
	const auto surf = TTF_RenderText_Blended(m_font->GetFont(), m_text.c_str(), color);
	if (surf == nullptr)
	{
		throw std::runtime_error(std::string("Render text failed: ") + SDL_GetError());
	}

	auto texture = SDL_CreateTextureFromSurface(dae::Renderer::GetInstance().GetSDLRenderer(), surf);
	if (texture == nullptr)
	{
		throw std::runtime_error(std::string("Create text texture from surface failed: ") + SDL_GetError());
	}
	SDL_FreeSurface(surf);

	m_texture = std::make_shared<dae::Texture2D>(texture);
	m_needsUpdate = false;

}

// This implementation uses the "dirty flag" pattern
void TextComponent::SetText(const std::string& text)
{
	m_text = text;
	m_needsUpdate = true;
}

void TextComponent::SetFont(std::shared_ptr<Font> font)
{
	m_font = std::move(font);
	m_needsUpdate = true;
}