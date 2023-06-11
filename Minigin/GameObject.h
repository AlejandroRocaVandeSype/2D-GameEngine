#ifndef GAME_ENGINE_GAMEOBJECT
#define GAME_ENGINE_GAMEOBJECT

#include <vector>
#include <type_traits>
#include "RenderComponent.h"
#include "TransformComponent.h"

namespace engine
{
	
	class GameObject final
	{
		
	public:
		
		GameObject(GameObject* pParent, const std::string& tag, glm::vec3 startPosition, glm::vec2 scale = { 1.f, 1.f},
			bool keepWorldPosition = true);
		~GameObject();
		GameObject(const GameObject& other) = delete;
		GameObject(GameObject&& other) = delete;
		GameObject& operator=(const GameObject& other) = delete;
		GameObject& operator=(GameObject&& other) = delete;

		void Update(const float deltaTime);
		void Render() const;
		void SendMessage(const std::string& message, const std::string& value);			// Send Message to components

		// ----------------------------------------------------
		// COMPONENTS ARCHITECTURE
		// ----------------------------------------------------
		template <typename T, typename... Args> T* AddComponent(Args&&... args);
		template <typename T> void RemoveComponent();
		template <typename T> T* GetComponent() const;

		// ----------------------------------------------------
		// SCENEGRAPH
		// ----------------------------------------------------
		void SetParent(GameObject* parent, bool keepWorldPosition = true);
		void RemoveParentFromChildren();
		void DeleteDeadChildren();
		bool HasChildren() const;
		bool HasParent() const;
		std::vector<engine::GameObject*> GetChildren();
		const GameObject* getParent() const;
		const glm::vec3 GetWorldPosition() const;
		void SetPositionDirty();
		void SavePreviousWorldPosition(const glm::vec3& prevWorldPos);

		template <typename T> bool HasComponentAlready() const;
		const bool IsMarkedAsDead() const;
		void SetParentActivity(bool activate);

		void SetIsActive(const bool isActive);
		bool IsActive() const;
		void MarkAsDead();

		std::string Tag() const;

	private:

		// SceneGraph 
		bool FreeChild(GameObject* child);						// Remove child from the container but do NOT destroy it from the scene
		void AddChild(GameObject* child);
		void DeleteChild(GameObject* child);					// Remove the child from the container and destroy it from the scene

		GameObject* m_pParent;
		std::vector<std::unique_ptr<GameObject>> m_vChildren;	// Parent will own his children

		// COMPONENTS
		std::vector<std::unique_ptr<engine::Component>> m_vComponents;  
		RenderComponent* m_pRenderCP;
		TransformComponent* m_pTransformCP;
		bool m_KeepWorldPosition;
		glm::vec3 m_PreviousWorldPosition;						// For the render in case the transformCP has been removed

		bool m_IsActive;
		bool m_IsDead;											// If the gameObject needs to be removed after updating all gameObjects
		bool m_HasToRender;										// Does this gameObject have a render component?
		bool m_IsParentDeactivated;

		std::string m_Tag;

	};

	// Look for the component in the container and returns if found. (Nullptr otherwise)
	template <typename T>
	inline T* engine::GameObject::GetComponent() const
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

	// Add a component avoiding duplicates
	template <typename T, typename... Args>
	inline T* GameObject::AddComponent(Args&&... args)
	{
		static_assert(std::is_base_of<engine::Component, T>::value, "Incorrect type passed to AddComponent function");
		
		if (HasComponentAlready<T>())
		{
			// Duplicate component -> Dont add it
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
	
	// Remove the component and send a message to all remaining components indicating the component that has been removed
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
			
				m_vComponents.erase(componentItr);							
				break;
			}
			
		}
	}

	// Check if the component is already in container
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

#endif