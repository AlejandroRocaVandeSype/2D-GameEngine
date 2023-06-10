#ifndef GAME_ENGINE_ANIMATIONCP
#define GAME_ENGINE_ANIMATIONCP
#include "Component.h"
#include <SDL.h>

namespace engine
{
	class RenderComponent;
	class AnimationCP final : public engine::Component
	{
	public:
		AnimationCP(engine::GameObject* pOwner, const int numberCols, const int totalFrames, const float frameRate);
		AnimationCP(engine::GameObject* pOwner, const int numberCols, const int totalFrames, const float frameRate, 
			int frameInc, int limitFrame, int startFrame = 0);
		~AnimationCP() override;

		virtual void Update(const float deltaTime) override;
		virtual void ReceiveMessage(const std::string& message, const std::string& value) override;

		const SDL_Rect& GetSpriteRect() const;

	private:

		void UpdateSourceRect();

		int m_CurrentFrame;				// Current frame being render
		int m_TotalFrames;				// Amount of frames from the spritesheet
		int m_LimitFrame;				// In case user doesn't want to render all frames
		float m_ElapsedFrameTime;
		float m_FrameRate;				
		int m_FrameInc;					// In case user wants to inc with different values than default (1)
		int m_TotalCols;
		int m_CurrentCol;
		int m_CurrentRow;

		engine::RenderComponent* m_pRenderComponent;
		SDL_Rect m_pSourceRect;
	};
}

#endif
