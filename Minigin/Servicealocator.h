#ifndef GAME_ENGINE_SERVICEALOCATOR
#define GAME_ENGINE_SERVICEALOCATOR

#include "SoundSystem.h"

namespace engine
{
	class Servicealocator final
	{
		static engine::SoundSystem* _ss_instance;
	public:

		// SOUND SYSTEM SERVICE
		static engine::SoundSystem& Get_Sound_System() { return *_ss_instance; }
		static void Register_sound_system(engine::SoundSystem* ss) { _ss_instance = ss;  }
	};
}


#endif

