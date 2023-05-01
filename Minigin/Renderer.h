#ifndef GAME_ENGINE_RENDERER
#define GAME_ENGINE_RENDERER

#include <SDL.h>
#include "Singleton.h"

class TrashTheCache;
namespace engine
{
	class Texture2D;
	/**
	 * Simple RAII wrapper for the SDL renderer
	 */
	class Renderer final : public Singleton<Renderer>
	{
		SDL_Renderer* m_renderer{};
		SDL_Window* m_window{};
		SDL_Color m_clearColor{};	

		// IMGUI
		bool m_Ex1ButtonPressed{ false };
		bool m_Ex2ButtonPressed{ false };
		bool m_ShowExercise1{ false };
		bool m_ShowExercise2{ false };
		float m_XValues[11]{ 1, 2, 4, 8, 16, 32, 64, 128, 256, 512, 1024 };

	public:
		void Init(SDL_Window* window);
		void Render();
		void Destroy();

		void RenderTexture(const Texture2D& texture, float x, float y) const;
		void RenderTexture(const Texture2D& texture, float x, float y, float width, float height) const;

		SDL_Renderer* GetSDLRenderer() const;

		const SDL_Color& GetBackgroundColor() const { return m_clearColor; }
		void SetBackgroundColor(const SDL_Color& color) { m_clearColor = color; }

	};
}

#endif
