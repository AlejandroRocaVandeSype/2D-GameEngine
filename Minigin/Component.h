#ifndef GAME_ENGINE_COMPONENT
#define GAME_ENGINE_COMPONENT
#include <string>


namespace engine
{
	class GameObject;
	class Texture2D;
	class Component
	{
	public:

		Component(const std::string& name, engine::GameObject* pOwner);
		virtual ~Component() = default;

		Component(const Component& other) = delete;
		Component(Component&& other) = delete;
		Component& operator=(const Component& other) = delete;
		Component& operator=(Component&& other) = delete;

		virtual void Update(const float deltaTime) = 0;
		virtual void ReceiveMessage(const std::string& message, const std::string& value) = 0;

		const std::string& GetName() const { return m_Tag; };

	private :
		engine::GameObject* m_pOwner;

	protected:
		std::string m_Tag;
		engine::GameObject* GetOwner() const {	return m_pOwner; };

	};
}

#endif