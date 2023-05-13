#ifndef GAME_ENGINE_SOUNDSYSTEM
#define GAME_ENGINE_SOUNDSYSTEM
#include <string>

namespace engine
{
	// SERVICE AUDIO SYSTEM INTERFACE
	class SoundSystem
	{
	public:
		virtual ~SoundSystem() = default;
		virtual void PlaySound(const short id) = 0;
		virtual void Update() = 0;
		virtual void CreateSound(const short id, const std::string& soundPath, const int volume) = 0;
	};
}

#endif

