#pragma once

#include "GameObject.h"
class RenderComponent;
class TextureObject
{
public:
	TextureObject(const std::string& filename, glm::vec3 position = glm::vec3{ 0.f, 0.f, 0.f });
	~TextureObject();

	std::shared_ptr<dae::GameObject> GameObject() const;
	void SetTexture(const std::string& filename);
	

private :

	std::shared_ptr<dae::GameObject> m_GameObject;
	RenderComponent* m_pRenderCP;
};

