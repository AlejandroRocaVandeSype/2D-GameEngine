#ifndef GAME_ENGINE_COMPONENT
#define GAME_ENGINE_COMPONENT
#include <string>
//#include <unordered_set>

// BASE CLASS FOR ALL THE COMPONENTS

/*
template<typename T>
struct Message {
	std::string message;
	T data;
};
*/
namespace dae {
	class GameObject;
	class Texture2D;
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

	virtual void Update(const float deltaTime) = 0;
	virtual void ReceiveMessage(const std::string& message, const std::string& value) = 0;
	/*
	template<typename T>
	void ReceiveMessage(const Message<T> message)
	{
		// Check if this component is interested in the given message type
		if (m_registeredMessageTypes.count(message.message) > 0)
		{
			// Cast the value to the appropriate type and handle the message
			HandleMessage(message, static_cast<const void*>(&message.data), sizeof(T));
		}
	}
	*/
	const std::string& GetName() const;

private :
	dae::GameObject* m_pOwner;
	//std::unordered_set<std::string> m_registeredMessageTypes;  // Fast to get the messages to check if component is interested on it

protected:
	std::string m_Name;
	dae::GameObject* GetOwner() const;
	// Handle a received message
	//virtual void HandleMessage(const std::string& message, const void* value, size_t size) = 0;
	

	
};

#endif