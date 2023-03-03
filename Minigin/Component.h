#pragma once
#include <string>

// BASE CLASS FOR ALL THE COMPONENTS


namespace dae {
	class GameObject;
}

class Component
{
public:

	Component(const std::string& name, dae::GameObject* pOwner);
	virtual ~Component() = default;

	Component(const Component& other) = delete;
	Component(Component&& other) = delete;
	Component& operator=(const Component& other) = delete;
	Component& operator=(Component&& other) = delete;

	virtual void Update([[maybe_unused]] const float deltaTime) = 0;
	virtual void ReceiveMessage(const std::string& message, const std::string& value) = 0;

	const std::string& GetName() const;

private :
	dae::GameObject* m_pOwner;

protected:
	std::string m_Name;

	
};

