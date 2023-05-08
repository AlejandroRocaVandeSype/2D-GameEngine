#ifndef GAME_ENGINE_PLAYERINPUTCP
#define GAME_ENGINE_PLAYERINPUTCP

#include "Component.h"

// SETUP ALL INPUT NEEDED IN ORDER TO CONTROL THE PLAYER CHARACTER
class PlayerInputCP final : public engine::Component
{
public:
	PlayerInputCP(engine::GameObject* pOwner);							// Using keyboard
	virtual ~PlayerInputCP() override;

	virtual void Update(const float deltaTime) override;
	virtual void ReceiveMessage(const std::string& message, const std::string& value) override;

	void AddControllerMovement();

	void SetPlayerDied();

private:
	bool m_PlayerDied;			// This is to ensure we unbind commands associated to the gameObject when it dies and not 
								// when program closes
	int m_ControllerIdx;		// Controller being used

};

#endif

