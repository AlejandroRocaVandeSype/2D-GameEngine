#include "SDL_SoundSystem.h"
#include <SDL_mixer.h>
#include <iostream>
#include <unordered_map>

class AudioClip final
{
public:
	AudioClip() {};
	~AudioClip() {};
private:
	Mix_Chunk* chunk;
};

class engine::SDL_SoundSystem::SDL_SoundSystem_Impl final
{
public:
	SDL_SoundSystem_Impl() 
	{
		// Initialize SDL_mixer with desired audio format (44.1kHz, 16-bit, stereo)
		int result = Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);
		if (result < 0) 
		{
			std::cerr << "Mix_OpenAudio failed: " << Mix_GetError() << std::endl;
		}

	
	}

	~SDL_SoundSystem_Impl()
	{
		for (auto& sound : m_Sounds)
		{
			Mix_FreeChunk(sound.second);
		}
	}

	void PlaySound( [[maybe_unused]] const engine::sound_id id, const int volume)
	{
		Mix_Chunk* sound_ = Mix_LoadWAV("../Data/PlayerShoot.wav");

		Mix_VolumeChunk(sound_, volume);

		if (!sound_) {
			std::cerr << "Mix_LoadWAV failed: " << Mix_GetError() << std::endl;
		}
		else
		{
			// -1 here means play in the first free channel
			
			int channel = Mix_PlayChannel(-1, sound_, 0);
			if (channel == -1) 
			{
				// There is no free channel
				std::cerr << "Mix_PlayChannel failed: " << Mix_GetError() << std::endl;
			}
		}

	}

private:
	std::unordered_map < std::string, Mix_Chunk*> m_Sounds;
};




engine::SDL_SoundSystem::SDL_SoundSystem()
{
	pImpl = std::make_unique<SDL_SoundSystem_Impl>();
}

void engine::SDL_SoundSystem::PlaySound(const engine::sound_id id, const int volume)
{
	pImpl->PlaySound(id, volume);
}



