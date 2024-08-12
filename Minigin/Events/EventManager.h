#ifndef ENGINE2D_MANAGER_H
#define ENGINE2D_MANAGER_H
#include "Singleton.h"
#include "Event.h"

#include <functional>
#include <vector>
#include <memory>
#include <unordered_map>


namespace dae
{
	class EventManager final : public Singleton<EventManager>
	{
	public:

		template <typename T>
		void QueueEvent(std::unique_ptr<T> e);

		void RegisterObserver(const Event& e, const std::function<void(const Event* e)>& observer);

		void HandleEvents();


	private:

		friend class Singleton<EventManager>;
		~EventManager() = default;

		bool DequeueEvent(Event*& e);

		const unsigned int m_QueueSize{ 20 };
		std::vector<std::unique_ptr<Event>> m_Queue{ m_QueueSize };
		int m_FrontIndex;
		int m_QueuedCount;

		std::unordered_map<Event, std::vector<std::function<void(const Event* e)>>> m_Observers;
	};


	template<typename T>
	inline void EventManager::QueueEvent(std::unique_ptr<T>  e)
	{
		static_assert(std::is_base_of<Event, T>::value && "T must inherit from Event");

		const unsigned currentIndex = (m_FrontIndex + m_QueuedCount) % m_QueueSize;
		m_Queue[currentIndex] = std::move(e);

		++m_QueuedCount;
	}
}
#endif //ENGINE2D_MANAGER_H