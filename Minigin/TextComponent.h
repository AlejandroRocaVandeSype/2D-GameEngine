#pragma once
#include "RenderComponent.h"

namespace dae {

	class Font;

	class TextComponent final : public RenderComponent
	{
	public:

		TextComponent();
		void Update([[maybe_unused]] const float deltaTime) override;

		void CreateTextureFromText();
		void SetText(const std::string& text);
		void SetFont(std::shared_ptr<Font> font);

	private:

		std::string m_text;
		std::shared_ptr<Font> m_font;
	};

}


