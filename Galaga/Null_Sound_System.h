#ifndef GAME_ENGINE_SERVICEALOCATOR
#define GAME_ENGINE_SERVICEALOCATOR

#include "SoundSystem.h"

namespace engine
{
	// Class that doesnt do anything. It is used as a Null provider when no sound system has 
	// been provided to the service alocator
	class Null_Sound_System final : engine::SoundSystem
	{
	public:
		void PlaySound(const short sound_id) override {};
	};

}

#endif

