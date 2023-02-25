#pragma once

#include <memory>
#include <glm/glm.hpp>
#include <string>
#include "Component.h"


namespace dae { 
	class Texture2D; 
}
class RenderComponent : public Component
{
public :
	RenderComponent();

	virtual void Update([[maybe_unused]] const float deltaTime) override;
	virtual void Render(const glm::vec3& position) const;

	void SetTexture(const std::string& filename);
	

protected :

	std::shared_ptr<dae::Texture2D> m_texture{};
	bool m_needsUpdate;
};

