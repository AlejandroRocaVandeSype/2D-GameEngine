#ifndef GALAGA_GAMECP
#define GALAGA_GAMECP

#include <Component.h>

// Component that contains all the Game States and updates them
class GameState;
class GameCP final : public engine::Component
{
public:

	GameCP(engine::GameObject* pOwner);
	virtual ~GameCP() override;

	virtual void Update(const float deltaTime) override;
	virtual void ReceiveMessage(const std::string& message, const std::string& value) override;
		
private:
	GameState* m_pGameState;

};

#endif
