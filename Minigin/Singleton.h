#ifndef GAME_ENGINE_SINGLETON
#define GAME_ENGINE_SINGLETON

namespace engine
{
	template <typename T>
	class Singleton
	{
	public:
		static T& GetInstance()
		{
			// This is thread safe (no need of mutex)
			static T instance{};
			return instance;
		}

		virtual ~Singleton() = default;
		Singleton(const Singleton& other) = delete;
		Singleton(Singleton&& other) = delete;
		Singleton& operator=(const Singleton& other) = delete;
		Singleton& operator=(Singleton&& other) = delete;

	protected:
		Singleton() = default; // Ensure no one can create another instance of the object

	};
}

#endif