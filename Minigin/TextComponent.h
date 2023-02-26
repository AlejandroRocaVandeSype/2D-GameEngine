#pragma once
#include <string>
#include <memory>
#include "Component.h"
#include "RenderComponent.h"

namespace dae {

	class Font;
	
	class TextComponent final : public Component
	{
	public:

		TextComponent(const std::string& text, std::shared_ptr<Font> font, RenderComponent* renderCP);
		void Update([[maybe_unused]] const float deltaTime) override;

		void CreateTextureFromText();
		void SetText(const std::string& text);


	private:

		std::string m_text;
		std::shared_ptr<Font> m_font;
		RenderComponent* m_RenderCP;
		bool m_needsUpdate;
	};

}


