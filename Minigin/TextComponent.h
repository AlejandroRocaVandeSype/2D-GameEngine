#pragma once
#include "RenderComponent.h"
#include <SDL_ttf.h>

namespace dae 
{

	class Font;
	
	class TextComponent final : public Component
	{
	public:

		TextComponent(GameObject* pOwner, const std::string& text, std::shared_ptr<Font> font, const SDL_Color& color = { 255, 255, 255 });
		~TextComponent();
		void Update(const float deltaTime) override;
		virtual void ReceiveMessage(const std::string& message, const std::string& value) override;
		void CreateTextureFromText();
		void SetText(const std::string& text);


	private:

		std::string m_text;
		std::shared_ptr<Font> m_font;
		RenderComponent* m_pRenderCP;
		SDL_Color m_Color;
		bool m_needsUpdate;
	};

}


