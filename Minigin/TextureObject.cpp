#include "TextureObject.h"
#include "RenderComponent.h"

TextureObject::TextureObject(const std::string& filename, glm::vec3 position)
{
	m_GameObject = std::make_shared<dae::GameObject>();
	m_pRenderCP = m_GameObject->AddComponent<RenderComponent>();
	if(m_pRenderCP != nullptr)
		m_pRenderCP->SetTexture(filename);

	if (position.x != 0 || position.y != 0)
	{
		// Not default position at (0,0) set the new Position
		auto t = m_GameObject->GetComponent<TransformComponent>();
		t->SetPosition(position);
	}
	

}

// Get the gameObject from this class with all the components attach it to it
std::shared_ptr<dae::GameObject> TextureObject::GameObject() const
{
	return m_GameObject;
}

void TextureObject::SetTexture(const std::string& filename)
{
	if (m_pRenderCP != nullptr)
		m_pRenderCP->SetTexture(filename);
}

TextureObject::~TextureObject()
{
	
}