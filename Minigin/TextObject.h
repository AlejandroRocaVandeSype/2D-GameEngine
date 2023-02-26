#pragma once
#include <string>
#include <memory>
#include "GameObject.h"
#include <SDL_ttf.h>

namespace dae
{
	class Font;
	class Texture2D;
	class GameObject;
	class TextComponent;

	class TextObject final
	{
	public:
		void Update([[maybe_unused]] const float deltaTime);
		void Render() const;

		void SetText(const std::string& text);
		//void SetPosition(float x, float y);

		TextObject(const std::string& text, std::shared_ptr<Font> font, glm::vec3 position, const SDL_Color& color = {255, 255, 255});
		virtual ~TextObject() = default;
		TextObject(const TextObject& other) = delete;
		TextObject(TextObject&& other) = delete;
		TextObject& operator=(const TextObject& other) = delete;
		TextObject& operator=(TextObject&& other) = delete;

		std::shared_ptr<dae::GameObject> GameObject() const;

		void AddFPSComponent();

	private:

		std::shared_ptr<dae::GameObject> m_GameObject;
		TextComponent* m_pTextCP;
		RenderComponent* m_pRenderCP;
		
	};
}
