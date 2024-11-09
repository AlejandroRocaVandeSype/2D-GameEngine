#ifndef GAME_ENGINE_TEXTCOMPONENT
#define GAME_ENGINE_TEXTCOMPONENT
#include "RenderComponent.h"
#include <SDL_ttf.h>

namespace engine
{
	class Font;
}
class TextComponent final : public engine::Component
{
	public:

		TextComponent(engine::GameObject* pOwner, const std::string& text, std::shared_ptr<engine::Font> font, const SDL_Color& color = { 255, 255, 255 });
		~TextComponent() override;
		void Update(const float deltaTime) override;
		virtual void ReceiveMessage(const std::string& message, const std::string& value) override;
		void CreateTextureFromText();
		void SetText(const std::string& text);
		std::string GetText() const;

	private:
		std::string m_text;
		std::shared_ptr<engine::Font> m_font;
		engine::RenderComponent* m_pRenderCP;
		SDL_Color m_Color;
		bool m_needsUpdate;
};

#endif
