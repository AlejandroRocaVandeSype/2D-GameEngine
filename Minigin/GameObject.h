#pragma once

#include <vector>
#include <memory>
#include <type_traits>

#include "TextComponent.h"
#include "TransformComponent.h"
#include <iostream>

namespace dae
{
	
	class GameObject final
	{
		
	public:
		
		GameObject();
		~GameObject();
		GameObject(const GameObject& other) = delete;
		GameObject(GameObject&& other) = delete;
		GameObject& operator=(const GameObject& other) = delete;
		GameObject& operator=(GameObject&& other) = delete;

		void Update([[maybe_unused]] const float deltaTime);
		void Render() const;

		template <typename T> T* AddComponent();
		void RemoveComponent(Component::ComponentType type);
		template <typename T> T* GetComponent() const;

		const bool HasARender() const;
		template <typename T> bool HasComponentAlready() const;

	private:


		std::vector<Component*> m_vComponents;
		RenderComponent* m_pRenderCP;
		TransformComponent* m_pTransformCP;

		bool m_IsActive;
		bool m_HasToRender;		// Does this gameObject have a render component?

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

	// Add component if it is not already in the container
	template <typename T>
	inline T* GameObject::AddComponent()
	{
		
		static_assert(std::is_base_of<Component, T>::value, "Incorrect type passed to AddComponent function");
		
		if (HasComponentAlready<T>())
		{
			// Component already added
			return nullptr;
		}

		T* component = new T();
		if (m_HasToRender == false)
		{		
			if (std::is_base_of<RenderComponent, T>::value)
			{
				// We attach a renderComponent to the gameObject
				m_HasToRender = true;
				m_pRenderCP = dynamic_cast<RenderComponent*>(component);
			}
			
		}

		m_vComponents.emplace_back(std::move(component));

		return component;
	}

	template <typename T>
	inline bool GameObject::HasComponentAlready() const
	{
		// It still doesnt check that if TextComponent is added, to not add a RenderComponent
		// Dont add the component if already added
		for (const auto& component : m_vComponents)
		{
			if (dynamic_cast<T*>(component))
			{
				return true;
			}
		}

		return false;
	}

	
	


}
