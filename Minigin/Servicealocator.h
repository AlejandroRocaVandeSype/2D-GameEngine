#ifndef GAME_ENGINE_SERVICEALOCATOR
#define GAME_ENGINE_SERVICEALOCATOR

#include "SoundSystem.h"
#include "Null_Sound_System.h"

namespace engine
{
	class Servicealocator final
	{		
	public:

		// SOUND SYSTEM SERVICE
		/*static void InitializeSoundSystem()
		{
			_ss_instance = &_default_ss;
		}*/
		static engine::SoundSystem& Get_Sound_System() { return *_ss_instance; }
		static void Register_sound_system(engine::SoundSystem* ss) 
		{ 
			if (ss == nullptr)
			{
				// If service provide is null -> We use or null object
				ss = &_default_ss;
			}

			_ss_instance = ss;

		}
		static void ShutDownSoundSystem()
		{
			if (_ss_instance != &_default_ss)
			{
				delete _ss_instance;
			}
			
			_ss_instance = &_default_ss;
		}


	private:
		static engine::SoundSystem* _ss_instance;
		static engine::Null_Sound_System _default_ss;
	};

	
}


#endif

