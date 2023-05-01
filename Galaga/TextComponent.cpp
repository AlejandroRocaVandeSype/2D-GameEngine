#include "TextComponent.h"
#include "Font.h"
#include "Texture2D.h"
#include "Renderer.h"
#include <stdexcept>
#include "GameObject.h"
#include <iostream>

TextComponent::TextComponent(engine::GameObject* pOwner, const std::string& text, std::shared_ptr<engine::Font> font, const SDL_Color& color)
	: Component("TextCP", pOwner)
	, m_text{ text }
	, m_font { font }
	, m_needsUpdate{ true }
	, m_Color{ color }

{
	if (pOwner != nullptr)
	{
		m_pRenderCP = pOwner->GetComponent<engine::RenderComponent>();
	}
	else
	{
		m_pRenderCP = nullptr;
	}
}

TextComponent::~TextComponent()
{
	std::cout << "TextComponent destructor" << std::endl;
}

void TextComponent::Update([[maybe_unused]] const float deltaTime)
{
	if (m_needsUpdate && m_pRenderCP != nullptr)
	{
		CreateTextureFromText();
	}
}

void TextComponent::ReceiveMessage(const std::string& message, const std::string& value)
{
	if (message == "RemoveCP")
	{
		if (value == "RenderCP")
		{
			m_pRenderCP = nullptr;
		}
	}
}

void TextComponent::CreateTextureFromText()
{
	//const SDL_Color color = { 255,255,255 }; // only white text is supported now
	const auto surf = TTF_RenderText_Blended(m_font->GetFont(), m_text.c_str(), m_Color);
	if (surf == nullptr)
	{
		throw std::runtime_error(std::string("Render text failed: ") + SDL_GetError());
	}

	auto texture = SDL_CreateTextureFromSurface(engine::Renderer::GetInstance().GetSDLRenderer(), surf);
	if (texture == nullptr)
	{
		throw std::runtime_error(std::string("Create text texture from surface failed: ") + SDL_GetError());
	}
	SDL_FreeSurface(surf);

	m_pRenderCP->SetTexture(std::make_shared<engine::Texture2D>(texture));
	m_needsUpdate = false;

}

// This implementation uses the "dirty flag" pattern
void TextComponent::SetText(const std::string& text)
{
	m_text = text;
	m_needsUpdate = true;
}
