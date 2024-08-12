#ifndef ENGINE2D_EVENT_H
#define ENGINE2D_EVENT_H
#include <string>


namespace dae
{
	// Define a base class for events
	struct Event
	{
		virtual ~Event() = default;
		const char* eventType{};
	};

	// Define a derived class for player events
	struct PlayerEvent : Event
	{
		PlayerEvent(const char* type, unsigned int Index)
			:playerIndex{ Index }
		{
			eventType = type;
		};

		unsigned int playerIndex{};
	};
	
	// Define a derived class for scene events
	struct SceneEvent : Event
	{
		std::string sceneName{};
		int nrPoints{};
	};

	// Define an equality operator for events
	inline bool operator==(const Event& lhs, const Event& rhs)
	{
		return lhs.eventType == rhs.eventType;
	}
}

namespace std
{
	// Specialize the std::hash template for the Event class
	template<>
	struct hash<dae::Event>
	{
		// Compute a hash value for an event
		std::size_t operator()(const dae::Event& input) const
		{
			// Use pointer arithmetic to compute a hash value for eventType
			std::size_t hashValue = 0;
			const char* eventTypePtr = input.eventType;
			while (*eventTypePtr)
			{
				// Shift the hash value left by 5 bits and add the current character
				hashValue = (hashValue << 5) + *eventTypePtr++;
			}
			return hashValue;
		}
	};

}

#endif // !ENGINE2D_EVENT_H