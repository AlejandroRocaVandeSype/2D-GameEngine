#ifndef MOVE_COMPONENT
#define MOVE_COMPONENT

#include "Component.h"
#include <vector>
#include "SDL.h"

/*  THIS COMPONENT CONTAINS ALL THE NECESSARY FUNCIONALITY TO MAKE A
	GAMEOBJECT MOVE USING THE KEYBOARD */
class MoveComponent final : public engine::Component
{
public:

	MoveComponent(engine::GameObject* pOwner, float speed);							// Using keyboard
	MoveComponent(engine::GameObject* pOwner, float speed, unsigned controllerIdx);	// Using controller
	virtual ~MoveComponent() override;

	virtual void Update(const float deltaTime) override;
	virtual void ReceiveMessage(const std::string& message, const std::string& value) override;

private :
	//std::vector<SDL_Keycode> m_Keys;
};

#endif