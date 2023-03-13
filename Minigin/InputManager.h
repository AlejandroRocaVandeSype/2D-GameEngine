#ifndef GAME_ENGINE_INPUTMANAGER
#define GAME_ENGINE_INPUTMANAGER

#include "Singleton.h"

namespace dae
{
	class InputManager final : public Singleton<InputManager>
	{
	public:
		bool ProcessInput();
	};

}

#endif