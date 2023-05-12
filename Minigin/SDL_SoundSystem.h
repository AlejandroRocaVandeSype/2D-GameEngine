#ifndef GAME_ENGINE_SDL_SOUNDSYSTEM
#define GAME_ENGINE_SDL_SOUNDSYSTEM

#include "SoundSystem.h"
#include <memory>

namespace engine
{
	class SDL_SoundSystem final : public engine::SoundSystem
	{
	public:
		SDL_SoundSystem();

		void PlaySound(const engine::sound_id id, const int volume) override;

	private:
		class SDL_SoundSystem_Impl;
		std::unique_ptr<SDL_SoundSystem_Impl> pImpl;

	};
}

#endif
