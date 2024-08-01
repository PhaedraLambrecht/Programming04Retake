#ifndef ENGINE2D_GAMEEVENTS_H
#define ENGINE2D_GAMEEVENTS_H
#include <Events/Event.h>

namespace dae
{
	struct PointEvent : public  Event
	{
		int nrPoints{};
	};

	class EnemyControllerComponent;
	struct ButterflyDestroyedEvent : public  Event
	{
		EnemyControllerComponent* Enemy{};
	};
}

#endif // !ENGINE2D_GAMEEVENTS_H