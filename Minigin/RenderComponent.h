#ifndef GAME_ENGINE_RENDERCOMPONENT
#define GAME_ENGINE_RENDERCOMPONENT

#include <memory>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <string>
#include "Component.h"

namespace engine
{
	class Texture2D;
	class AnimationCP;
	class RenderComponent final : public Component
	{
	public:

		RenderComponent(engine::GameObject* pOwner);
		RenderComponent(engine::GameObject* pOwner, const std::string& filename);
		~RenderComponent() override;
		virtual void Update(const float deltaTime) override;
		virtual void Render(const glm::vec3& position) const;
		virtual void ReceiveMessage(const std::string& message, const std::string& value) override;

		void SetTexture(const std::string& filename);
		void SetTexture(std::shared_ptr<engine::Texture2D> texture);
		void SetAnimationCP(engine::AnimationCP* animationCP);

		const glm::vec2 GetTextureSize();
		const glm::vec2 GetSpriteSize() const;
		const glm::vec2& GetScale();

	private:

		std::shared_ptr<engine::Texture2D> m_texture;
		glm::vec2 m_Scale;
		glm::vec2 m_TextureSize;
		bool m_IsTextureDirty;								// To indicate if the texture changed or not
	
		engine::AnimationCP* m_pAnimationCP;				// For animated sprites only		

		void SetScale();
		void CalculateTextureSize();
	};
}

#endif
