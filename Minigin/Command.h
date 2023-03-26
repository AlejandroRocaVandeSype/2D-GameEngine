#ifndef GAME_ENGINE_COMMAND
#define GAME_ENGINE_COMMAND

namespace dae
{
	class GameObject;
}

// COMMAND PATTERN FOR INPUT MANAGEMENT
class Command
{
public :

	explicit Command(dae::GameObject* actor);
	Command(const Command& other) = delete;
	Command(Command&& other) = delete;
	Command& operator=(const Command& other) = delete;
	Command& operator=(Command&& other) = delete;
	virtual ~Command() = default;

	virtual void Execute(float deltaTime) = 0; 
	

protected:
	dae::GameObject* GetActor() const { return m_Actor; }

private : 
	dae::GameObject* m_Actor;   

}; 

#endif