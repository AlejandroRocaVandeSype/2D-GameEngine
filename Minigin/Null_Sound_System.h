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
		void PlaySound([[maybe_unused]] const short id) override {};
		void ProcessRequests() override {};
		void RegisterSoundID([[maybe_unused]] const short id, [[maybe_unused]] const std::string& soundPath, [[maybe_unused]] const int volume) override {};
	};

}

#endif

