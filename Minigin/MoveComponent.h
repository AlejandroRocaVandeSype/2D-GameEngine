#ifndef MOVE_COMPONENT
#define MOVE_COMPONENT

#include "Component.h"
#include <vector>
#include "SDL.h"

/*  THIS COMPONENT CONTAINS ALL THE NECESSARY FUNCIONALITY TO MAKE A
	GAMEOBJECT MOVE USING THE KEYBOARD */
class MoveComponent : public Component
{
public:

	MoveComponent(dae::GameObject* pOwner, float speed);
	virtual ~MoveComponent() override;

	virtual void Update(const float deltaTime) override;
	virtual void ReceiveMessage(const std::string& message, const std::string& value) override;

private :
	std::vector<SDL_Keycode> m_Keys;
};

#endif