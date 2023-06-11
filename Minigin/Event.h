#ifndef GAME_ENGINE_EVENT
#define GAME_ENGINE_EVENT

#include <string>

namespace engine
{
	// For the Observer class. In order to know which type of event was sent
	class Event final
	{

		public:
			explicit Event(const std::string& eventType);
			~Event() = default;
			Event(const Event& other) = delete;
			Event(Event&& other) = delete;
			Event& operator=(const Event& other) = delete;
			Event& operator=(Event&& other) = delete;

			const std::string& GetType() const;
			bool IsSameEvent(const std::string& eventType) const;

		private:
			std::string m_Type;				// Type of the event

	};
}

#endif