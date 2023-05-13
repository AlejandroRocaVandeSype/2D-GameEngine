#ifndef GAME_ENGINE_LOGGING_SOUND_SYSTEM
#define GAME_ENGINE_LOGGING_SOUND_SYSTEM

#include "SoundSystem.h"
#include <iostream>

namespace engine
{
	// FOR DEBUGGING PURPOSES
	// LOGS EACH SOUND CALL TO THE REAL SOUND SYSTEM WE ARE USING
	class Logging_Sound_System final : public engine::SoundSystem
	{
	public:

		// Wrap the real audio provider and use the same interface
		Logging_Sound_System(engine::SoundSystem* ss) : _real_ss(ss) {};
		~Logging_Sound_System() override
		{ 
			delete _real_ss;  
		}

		void PlaySound(const short id) override
		{
			_real_ss->PlaySound(id);
			std::cout << "Playing Sound " << id << "\n";
		}

		void Update() override { _real_ss->Update(); }

		void CreateSound(const short id, const std::string& soundPath, const int volume) override
		{
			_real_ss->CreateSound(id, soundPath, volume);
			std::cout << "Sound created " << id << " with Path " << soundPath << "and volume " << volume << "\n";
		}

	private:
		engine::SoundSystem* _real_ss;
	};
}

#endif


