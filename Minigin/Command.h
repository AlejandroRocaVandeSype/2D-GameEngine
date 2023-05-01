#ifndef GAME_ENGINE_COMMAND
#define GAME_ENGINE_COMMAND

namespace dae
{
	class GameObject;
}

// COMMAND PATTERN INTERFACE
class Command
{
public :

	explicit Command() {};
	Command(const Command& other) = delete;
	Command(Command&& other) = delete;
	Command& operator=(const Command& other) = delete;
	Command& operator=(Command&& other) = delete;
	virtual ~Command() = default;

	virtual void Execute(float deltaTime) = 0; 
	
}; 

#endif