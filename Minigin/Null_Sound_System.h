#ifndef GAME_ENGINE_NULL_SOUND_SYSTEM
#define GAME_ENGINE_NULL_SOUND_SYSTEM

#include "SoundSystem.h"


namespace engine
{
	// Class that doesnt do anything. It is used as a Null provider when no sound system has 
	// been provided to the service alocator
	class Null_Sound_System final : public engine::SoundSystem
	{
	public:
		void PlaySound( const short) override {};
		void ProcessRequests() override {};
		void RegisterSoundID( const short,  const std::string&,  const int) override {};
	};

}

#endif

