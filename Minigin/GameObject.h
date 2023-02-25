#pragma once

#include <vector>
#include <memory>
#include <type_traits>

#include "RenderComponent.h"
#include "TransformComponent.h"

namespace dae
{
	
	class GameObject final
	{
		
	public:
		

		//void SetTexture(const std::string& filename);
		//void SetPosition(float x, float y);

		GameObject();
		~GameObject();
		GameObject(const GameObject& other) = delete;
		GameObject(GameObject&& other) = delete;
		GameObject& operator=(const GameObject& other) = delete;
		GameObject& operator=(GameObject&& other) = delete;

		void Update([[maybe_unused]] const float deltaTime);
		void Render() const;

		template <typename T> T* AddComponent();
		template <typename T> void RemoveComponent(T* component);
		template <typename T> T* GetComponent() const;

		const bool HasRender() const;

	private:

		std::vector<Component*> m_vComponents;
		RenderComponent* m_pRenderCP;
		TransformComponent* m_pTransformCP;

		bool m_IsActive;
		bool m_HasRender;		// Does this gameObject have a render component?
	};

	template <typename T>
	inline T* dae::GameObject::GetComponent() const
	{
		for (auto& componentItr : m_vComponents)
		{
			auto casted = dynamic_cast<T*>(componentItr);
			if(casted)
			{
				// Component found
				return casted;
			}
		}

		// Component not found
		return nullptr;
	}

	template <typename T>
	inline void GameObject::RemoveComponent(T* component)
	{
		// Remember to put hasRender to false and the int to negative when renderComponent is deleted
		for (auto& componentItr : m_vComponents)
		{
			if (componentItr == component)
			{
				m_vComponents.erase(componentItr);
			}
		}
	}

	template <typename T>
	inline T* GameObject::AddComponent()
	{
		// Check if component already added or not
		static_assert(std::is_base_of<Component, T>::value, "Incorrect type passed to AddComponent function");
		
		T* component = new T();
		if (m_HasRender == false)
		{		
			if (std::is_base_of<RenderComponent, T>::value)
			{
				// We attach a renderComponent to the gameObject
				m_HasRender = true;
				m_pRenderCP = dynamic_cast<RenderComponent*>(component);
			}
			
		}

		m_vComponents.emplace_back(std::move(component));

		return component;
	}

	
	


}
