#include "EventManager.h"
#include <iostream>


// Add an observer function for the given event type
void dae::EventManager::RegisterObserver(const Event& e, const std::function<void(const Event* e)>& observer)
{
	auto& observerIt = m_Observers[e];
	observerIt.push_back(observer);
}

// Handle all events in the queue by notifying registered observers
void dae::EventManager::HandleEvents()
{
	Event* e{ nullptr };
	while (DequeueEvent(e))
	{
		auto observerIterator = m_Observers.find(*e);
	
		// If observers are registered for this event type, notify them
		if (observerIterator != m_Observers.end())
		{			
			for (const auto& observer : observerIterator->second)
			{
				observer(e);
			}
		}
	}
}

// Dequeue an event from the event queue and update the pointer
bool dae::EventManager::DequeueEvent(Event*& e)
{
	if (m_QueuedCount == 0)
	{
		return false;
	}

	e = m_Queue[m_FrontIndex].get();
	m_FrontIndex = (m_FrontIndex + 1) % m_QueueSize;
	--m_QueuedCount;

	return true;
}
