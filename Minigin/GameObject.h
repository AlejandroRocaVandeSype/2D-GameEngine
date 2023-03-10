#pragma once

#include <vector>
#include <type_traits>
#include "RenderComponent.h"
#include "TransformComponent.h"

namespace dae
{
	
	class GameObject final
	{
		
	public:
		
		GameObject(GameObject* pParent, glm::vec3 startPosition = glm::vec3{ 0.f, 0.f, 0.f });
		~GameObject();
		GameObject(const GameObject& other) = delete;
		GameObject(GameObject&& other) = delete;
		GameObject& operator=(const GameObject& other) = delete;
		GameObject& operator=(GameObject&& other) = delete;

		void Update([[maybe_unused]] const float deltaTime);
		void Render() const;
		void SendMessage(const std::string& message, const std::string& value);

		// COMPONENTS GAMEOBJECT
		template <typename T, typename... Args> T* AddComponent(Args&&... args);
		template <typename T> void RemoveComponent();
		template <typename T> T* GetComponent() const;

		// SCENEGRAPH
		void SetParent(GameObject* parent, bool keepWorldPosition = true);
		void SetPositionDirty();
		void UpdateChildrenPosition();
		void IncCountDeadChildren();
		void RemoveDeadChildren();
		const GameObject* getParent() const;
		bool HasChildren() const;
		bool HasParent() const;
		const glm::vec3 GetWorldPosition() const;
		
		const bool HasARender() const;
		template <typename T> bool HasComponentAlready() const;
		const bool IsMarkedAsDead() const;

		void SetIsActive(const bool isActive);
		void MarkAsDead();

	private:

		// SceneGraph 
		bool FreeChild(GameObject* child);						// Remove child from the container but not destroy it from the scene
		void AddChild(GameObject* child);
		void DestroyChild(GameObject* child);						// Remove the child from the container and destroy it from the scene

		GameObject* m_pParent;
		std::vector<std::unique_ptr<GameObject>> m_vChildren;		// Parent will own his children
		int m_TotalDeadChildren;									// Keep track of how many children are "dead"

		// Components of the GameObject
		std::vector<std::unique_ptr<Component>> m_vComponents;  
		RenderComponent* m_pRenderCP;
		TransformComponent* m_pTransformCP{};

		bool m_IsActive;
		bool m_IsDead;			// If the gameObject needs to be removed after updating all gameObjects
		bool m_HasToRender;		// Does this gameObject have a render component?

	};

	// Look for the component in the container and returns if found. (Nullptr otherwise)
	template <typename T>
	inline T* dae::GameObject::GetComponent() const
	{
		for (auto& componentItr : m_vComponents)
		{
			auto casted = dynamic_cast<T*>(componentItr.get());
			if (casted)
			{
				// Component found
				return casted;
			}
		}

		// Component not found
		return nullptr;
	}

	template <typename T, typename... Args>
	inline T* GameObject::AddComponent(Args&&... args)
	{
		static_assert(std::is_base_of<Component, T>::value, "Incorrect type passed to AddComponent function");

		
		if (HasComponentAlready<T>())
		{
			// Component already added
			return nullptr;
		}
		
		// Create an instance of the component with the corresponding args
		auto component = std::make_unique<T>(std::forward<Args>(args)...);
		T* rawPtr = component.get();

		if (m_HasToRender == false)
		{
			if (std::is_base_of<RenderComponent, T>::value)
			{
				// We attach a renderComponent to the gameObject
				m_HasToRender = true;
				m_pRenderCP = dynamic_cast<RenderComponent*>(rawPtr);
			}

		}

		m_vComponents.emplace_back(std::move(component));

		return rawPtr;

	}
	
	template <typename T> 
	inline void GameObject::RemoveComponent()
	{
		for (auto componentItr = m_vComponents.begin(); componentItr != m_vComponents.end(); ++componentItr)
		{
			auto component = dynamic_cast<T*>(componentItr->get());
			if (component)
			{
				
				if (std::is_base_of<RenderComponent, T>::value)
				{
					m_HasToRender = false;
					m_pRenderCP = nullptr;
				}

				if (std::is_base_of<TransformComponent, T>::value)
				{
					m_pTransformCP = nullptr;
				}
				
				SendMessage("RemoveCP", component->GetName());
			
				m_vComponents.erase(componentItr);  // With smart pointers this enough to delete the object
			
				break;
			}
			
		}
	}

	
	template <typename T>
	inline bool GameObject::HasComponentAlready() const
	{
		// Dont add the component if already added
		for (const auto& component : m_vComponents)
		{
			if (dynamic_cast<T*>(component.get()))
			{
				return true;
			}
		}

		return false;
	}

	
	


}
