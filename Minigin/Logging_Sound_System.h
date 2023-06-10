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
			std::cout << "Deleting sound system..." << std::endl;
			delete _real_ss;  
		}

		void PlaySound(const short id) override
		{
			_real_ss->PlaySound(id);
			if (!_real_ss->IsMuted())
			{
				std::cout << "Playing Sound " << id << "\n";
			}
			
		}

		void ToggleSoundSystemSound() override
		{
			_real_ss->ToggleSoundSystemSound();

			if (_real_ss->IsMuted())
			{
				std::cout << "Sound Muted" << std::endl;
			}
			else
			{
				std::cout << "Sound Unmuted" << std::endl;
			}
		}

		void ProcessRequests() override { _real_ss->ProcessRequests(); }

		void RegisterSoundID(const short id, const std::string& soundPath, const int volume) override
		{
			_real_ss->RegisterSoundID(id, soundPath, volume);
			std::cout << "Sound created " << id << " with Path " << soundPath << "and volume " << volume << "\n";
		}

		bool IsMuted() const override
		{
			return _real_ss->IsMuted();
		}

	private:
		engine::SoundSystem* _real_ss;
	};
}

#endif


