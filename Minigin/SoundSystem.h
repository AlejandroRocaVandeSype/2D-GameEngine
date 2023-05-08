#ifndef GAME_ENGINE_SOUNDSYSTEM
#define GAME_ENGINE_SOUNDSYSTEM

namespace engine
{
	// SERVICE AUDIO SYSTEM INTERFACE
	using sound_id = unsigned short;
	class SoundSystem
	{
	public:
		virtual ~SoundSystem() = default;
		virtual void play(const sound_id, const float volume) = 0;
	};
}

#endif

