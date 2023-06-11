#include "GameObject.h"
#include "AnimationCP.h"
#include "RenderComponent.h"

engine::AnimationCP::AnimationCP(engine::GameObject* pOwner, const int numberCols, const int totalFrames, const float frameRate)
	:Component("AnimationCP", pOwner)
	, m_CurrentFrame { 0 }, m_ElapsedFrameTime{ 0.f }, m_FrameRate { frameRate }, m_TotalFrames { totalFrames }
	, m_CurrentCol { 0 }, m_CurrentRow { 0 }, m_TotalCols { numberCols }, m_pRenderComponent { nullptr }
	, m_FrameInc{ 1 }, m_LimitFrame{ m_TotalFrames - 1 }, m_StartFrame{ 0 }, m_AnimationMode { AnimationMode::normal}
	, m_NormalState { true }
{

	m_pRenderComponent = pOwner->GetComponent<engine::RenderComponent>();

	if (m_pRenderComponent != nullptr)
	{
		auto textureSize = m_pRenderComponent->GetTextureSize();
		m_pSourceRect.w = int(textureSize.x / m_TotalCols);
		m_pSourceRect.h = int(textureSize.y / (m_TotalFrames / m_TotalCols));

		auto textureScale = m_pRenderComponent->GetScale();
		m_pSourceRect.x = int((m_pSourceRect.w / textureScale.x) * m_CurrentCol);
		m_pSourceRect.y = int((m_pSourceRect.h / textureScale.y) * m_CurrentRow);

		m_pRenderComponent->SetAnimationCP(this);
	}
	
}

// Extra parameters for the Animation like custom frame increment, until which frame render, different startFrame etc
engine::AnimationCP::AnimationCP(engine::GameObject* pOwner, const int numberCols, const int totalFrames, const float frameRate,
	int frameInc, int limitFrame, int startFrame, const AnimationMode& mode)
	: AnimationCP(pOwner, numberCols, totalFrames, frameRate)
{
	m_FrameInc = frameInc;
	m_LimitFrame = limitFrame;
	m_CurrentFrame = startFrame;
	m_StartFrame = startFrame;
	m_AnimationMode = mode;
}

engine::AnimationCP::~AnimationCP()
{

}

void engine::AnimationCP::Update(const float deltaTime)
{

	m_ElapsedFrameTime += deltaTime;

	// Check if the elapsed time is larger than the framerate
	if (m_ElapsedFrameTime > m_FrameRate)
	{
		//// Elapsed time more than framerate --> Next frame 
		switch (m_AnimationMode)
		{
			case engine::AnimationCP::AnimationMode::normal:
				NormalUpdate();
				break;
			case engine::AnimationCP::AnimationMode::backwards:
				BackwardsUpdate();
				break;
			case engine::AnimationCP::AnimationMode::normalAndBack:
			{
				// Toggle between both types
				if (m_NormalState)
				{
					NormalUpdate();
				}
				else
				{
					BackwardsUpdate();
				}
				break;
			}			
		}

		m_ElapsedFrameTime -= m_FrameRate;
		UpdateSourceRect();
	}

}

// Incrementing of the frames
void engine::AnimationCP::NormalUpdate()
{
	m_CurrentFrame += m_FrameInc;
	if (m_CurrentFrame > m_LimitFrame)
	{	
		if (m_AnimationMode == engine::AnimationCP::AnimationMode::normalAndBack)
		{
			SwapStarAndLimit();
		}
		else
		{
			// Reset to start if we reached the limit
			m_CurrentFrame = 0;
		}	
	}
}

// Decrement 
void engine::AnimationCP::BackwardsUpdate()
{
	m_CurrentFrame -= m_FrameInc;
	if (m_CurrentFrame < m_LimitFrame)
	{
		if (m_AnimationMode == engine::AnimationCP::AnimationMode::normalAndBack)
		{
			SwapStarAndLimit();
		}
		else
		{
			// Reset to start if we reached the limit
			m_CurrentFrame = m_StartFrame;
		}
	}
}

// For the normalAndBack mode so it franes are continuosly inc and dec
void engine::AnimationCP::SwapStarAndLimit()
{
	m_NormalState = !m_NormalState;
	int saveStart = m_StartFrame;
	m_StartFrame = m_LimitFrame;
	m_LimitFrame = saveStart;
}

// Updates the source rectangle according to the current frame being displayed
void engine::AnimationCP::UpdateSourceRect()
{
	m_CurrentRow = m_CurrentFrame / static_cast<int>(m_TotalCols);
	m_CurrentCol = m_CurrentFrame % static_cast<int>(m_TotalCols);

	auto textureScale = m_pRenderComponent->GetScale();
	m_pSourceRect.x = int((m_pSourceRect.w / textureScale.x) * m_CurrentCol);
	m_pSourceRect.y = int((m_pSourceRect.h / textureScale.y) * m_CurrentRow);
}

const SDL_Rect& engine::AnimationCP::GetSpriteRect() const
{
	return m_pSourceRect;
}

void engine::AnimationCP::ReceiveMessage(const std::string& message, const std::string& value)
{
	if (message == "RemoveCP")
	{
		if (value == "RenderCP")
		{
			m_pRenderComponent = nullptr;
		}
	}
}
