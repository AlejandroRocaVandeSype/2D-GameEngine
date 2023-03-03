#pragma once

#include <memory>
#include <glm/glm.hpp>
#include <string>
#include "Component.h"


namespace dae
{
	class Texture2D;
	class RenderComponent final : public Component
	{
	public:
		RenderComponent(dae::GameObject* pOwner);
		RenderComponent(dae::GameObject* pOwner, const std::string& filename);
		~RenderComponent();
		virtual void Update([[maybe_unused]] const float deltaTime) override;
		virtual void Render(const glm::vec3& position) const;
		virtual void ReceiveMessage(const std::string& message, const std::string& value) override;

		void SetTexture(const std::string& filename);
		void SetTexture(std::shared_ptr<dae::Texture2D> texture);

	private:

		std::shared_ptr<dae::Texture2D> m_texture{};

	};
}


