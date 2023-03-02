#pragma once

#include <vector>
#include <type_traits>
#include "RenderComponent.h"
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
		void SendMessage(const std::string& message, const std::string& value);
		void Destroy();

		template <typename T, typename... Args> T* AddComponent(Args&&... args);
		template <typename T> void RemoveComponent();
		template <typename T> T* GetComponent() const;

		const bool HasARender() const;
		//template <typename T> bool HasComponentAlready() const;
		const bool IsDead() const;

		void SetIsActive(const bool isActive);
		void SetIsDead(const bool isDead);

	private:


		std::vector<Component*> m_vComponents;
		RenderComponent* m_pRenderCP;
		TransformComponent* m_pTransformCP;

		bool m_IsActive;
		bool m_IsDead;
		bool m_HasToRender;		// Does this gameObject have a render component?

	};

	// Look for the component in the container and returns if found. (Nullptr otherwise)
	template <typename T>
	inline T* dae::GameObject::GetComponent() const
	{
		for (auto& componentItr : m_vComponents)
		{
			auto casted = dynamic_cast<T*>(componentItr);
			if (casted)
			{
				// Component found
				return casted;
			}
		}

		// Component not found
		return nullptr;
	}

	// Add the component in the container
	// If component already inside, returns nullptr and it doesnt add it
	template <typename T, typename... Args>
	inline T* GameObject::AddComponent(Args&&... args)
	{
		static_assert(std::is_base_of<Component, T>::value, "Incorrect type passed to AddComponent function");

		/*
		if (HasComponentAlready<T>())
		{
			// Component already added
			return nullptr;
		}
		*/
		// Create an instance of the component with the corresponding args
		T* component = new T(std::forward<Args>(args)...);
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
	inline void GameObject::RemoveComponent()
	{
		for (auto componentItr = m_vComponents.begin(); componentItr != m_vComponents.end(); ++componentItr)
		{
			auto component = dynamic_cast<T*>(*componentItr);
			if (component)
			{
				
				if (std::is_base_of<RenderComponent, T>::value)
				{
					m_HasToRender = false;
					m_pRenderCP = nullptr;
				}
				
				SendMessage("RemoveComponent", component->GetName());
				delete* componentItr;
				m_vComponents.erase(componentItr);  // With smart pointers this enough to delete the object
			
				break;
			}
			
		}
	}

	/*
	template <typename T>
	inline bool GameObject::HasComponentAlready() const
	{
		// Dont add the component if already added
		for (const auto& component : m_vComponents)
		{
			if (dynamic_cast<T>(component))
			{
				return true;
			}
		}

		return false;
	}
	*/
	
	


}
