#ifndef ENGINE2D_EVENT_H
#define ENGINE2D_EVENT_H
#include <string>


namespace dae
{
	struct Event
	{
		virtual ~Event() = default;
		const char* eventType{};
	};

	struct PlayerEvent : Event
	{
		PlayerEvent(const char* type, unsigned int Index)
			:playerIndex{ Index }
		{
			eventType = type;
		};
		unsigned int playerIndex{};
	};

	struct SceneEvent : Event
	{
		std::string sceneName{};
		int nrPoints{};
	};


	inline bool operator==(const Event& lhs, const Event& rhs)
	{
		return lhs.eventType == rhs.eventType;
	}
}

namespace std
{
	template<>
	struct hash<dae::Event>
	{
		std::size_t operator()(const dae::Event& input) const
		{
			// Use pointer arithmetic to compute a hash value for eventType
			std::size_t hashValue = 0;
			const char* eventTypePtr = input.eventType;
			while (*eventTypePtr)
			{
				hashValue = (hashValue << 5) + *eventTypePtr++;
			}
			return hashValue;
		}
	};

}

#endif // !ENGINE2D_EVENT_H