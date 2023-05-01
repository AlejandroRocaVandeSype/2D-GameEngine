#ifndef GAME_ENGINE_FPSCOMPONENT
#define GAME_ENGINE_FPSCOMPONENT
#include "Component.h"



class TextComponent;
class FPSComponent final : public engine::Component
{
	public:

		FPSComponent(engine::GameObject* pOwner);
		virtual ~FPSComponent() override;

		void Update(const float deltaTime) override;
		virtual void ReceiveMessage(const std::string& message, const std::string& value) override;

	private:

		unsigned int m_FrameCount;
		float m_FpsTimer;
		const float MAX_SECOND;

		TextComponent* m_pTextCP;

};

#endif

